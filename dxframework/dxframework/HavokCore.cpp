#include "HavokCore.h"

// Keycode
#include <Common/Base/keycode.cxx>

#undef HK_FEATURE_PRODUCT_AI
#undef HK_FEATURE_PRODUCT_ANIMATION
#undef HK_FEATURE_PRODUCT_CLOTH
#undef HK_FEATURE_PRODUCT_DESTRUCTION
#undef HK_FEATURE_PRODUCT_BEHAVIOR
#define HK_FEATURE_REFLECTION_PHYSICS
#define HK_CLASSES_FILE <Common/Serialize/Classlist/hkClasses.h>
#define HK_EXCLUDE_FEATURE_MemoryTracker
#define HK_EXCLUDE_FEATURE_SerializeDeprecatedPre700
#define HK_EXCLUDE_FEATURE_RegisterVersionPatches 
#define HK_EXCLUDE_LIBRARY_hkGeometryUtilities

#include <Common/Base/Config/hkProductFeatures.cxx>


static void HK_CALL errorReport(const char* msg, void* userArgGivenToInit)
{
	printf("%s", msg);
}

HavokCore::HavokCore(bool vdbEnabled)
{
	_vdbEnabled = vdbEnabled;

	initHavok();
}


HavokCore::~HavokCore(void)
{
	deinitHavok();
}

void HavokCore::initHavok()
{
	initMemory();
	initPhysicsWorld();

	if(_vdbEnabled)
		initVDB();
}

void HavokCore::initMemory()
{
	// Initialize the base system including our memory system
	// Allocate 0.5MB of physics solver buffer / Put Aside 0.5MB for physics
	hkMemoryRouter* memoryRouter = hkMemoryInitUtil::initDefault(hkMallocAllocator::m_defaultMallocAllocator, hkMemorySystem::FrameInfo(500 * 1024));
	hkBaseSystem::init(memoryRouter, errorReport);

	// We can cap the number of threads used - here we use the maximum for whatever multithreaded platform we are running on.
	// This variable is set here.
	int totalNumThreadsUsed;

	// Get the number of physical threads available on the system
	hkHardwareInfo hwInfo;
	hkGetHardwareInfo(hwInfo);
	totalNumThreadsUsed = hwInfo.m_numThreads;

	// We use one less than this for our thread pool, because we must also use this thread for our simulation
	hkCpuJobThreadPoolCinfo threadPoolCinfo;
	threadPoolCinfo.m_numThreads = totalNumThreadsUsed - 1;

	// This line enables timers collection, by allocating 200 kb per thread.
	// If you leave this at it's default (0), timer collection will not be enabled.
	threadPoolCinfo.m_timerBufferPerThreadAllocation = 200000;
	_threadPool = new hkCpuJobThreadPool(threadPoolCinfo);

	// We also need to create Job queue. This queue will be used by all of Havok modules to run multithreaded work.
	// Here we only use it for physics
	hkJobQueueCinfo info;
	info.m_jobQueueHwSetup.m_numCpuThreads = totalNumThreadsUsed;
	_jobQueue = new hkJobQueue(info);

	// Enable monitor for this thread
	// Monitors have been enabled for thread pool threads already.
	hkMonitorStream::getInstance().resize(200000);
}

void HavokCore::initPhysicsWorld()
{
	// Set the simulation type of the world to multi-threaded
	_pWorldInfo.m_simulationType = hkpWorldCinfo::SIMULATION_TYPE_MULTITHREADED;

	// Set the gravity
	_pWorldInfo.m_gravity.set(0.0f, -9.8f, 0.0f);

	// Flag objects that fall "out of the world" to be automatically removed
	_pWorldInfo.m_broadPhaseBorderBehaviour = hkpWorldCinfo::BROADPHASE_BORDER_REMOVE_ENTITY;

	// Set the world using the worldinfo just set
	_pWorld = new hkpWorld(_pWorldInfo);

	// Disable deactivation, so that you can view timers on the visual debugger.
	// This should not be done in your game
	_pWorld->m_wantDeactivation = false;

	// When the simulation type = SIMULATION_TYPE_MULTITHREADED, in the debug build, sdk performs checks
	// to make sure only one thread is modifying the world at once to prevent multithreaded bugs.
	// Each thread must call "markForRead" / "markForWrite" before it modifies the world to enable these checks.
	_pWorld->markForWrite();

	// Register all collision agents
	// It's important to register collision agents before adding any entities to the world
	hkpAgentRegisterUtil::registerAllAgents(_pWorld->getCollisionDispatcher());


	// We need to register all modules we will be running multi-threaded with the job queue
	_pWorld->registerWithJobQueue(_jobQueue);
}

// Initialize the Visual Debugger!
void HavokCore::initVDB()
{
	hkArray<hkProcessContext*> contexts;

	// <Physics-Only>: Register physics specific visual debugger processes
	// By default the visual debugger will show debug points and line, however some products such as physics and cloth have additional viewers
	// that can show geometries ect. and can be enabled and disabled by the visual debugger app.
	{
			// Initialize the visual debugger so we can connect remotely to the simulation
			// The context must exist beyond the use of the visual debugger instance, and you can make
			// whatever context you like for your own viewer types

			_context = new hkpPhysicsContext();
			hkpPhysicsContext::registerAllPhysicsProcesses();	// All the physics Viewers
			_context->addWorld(_pWorld);						// Add the physics world so the viewers can see it
			contexts.pushBack(_context);

			// Now we have finished modifying the world, release our write marker
			_pWorld->unmarkForWrite();
	}

	_vdb = new hkVisualDebugger(contexts);
	_vdb->serve();
}

// Deinitialize Havok
void HavokCore::deinitHavok()
{
	// Cleanup Physics
	_pWorld->markForWrite();
	_pWorld->removeReference();

	delete _jobQueue;

	// Clean Up the thread pool
	_threadPool->removeReference();

	if(_vdbEnabled)
		deinitVDB();

	hkBaseSystem::quit();
	hkMemoryInitUtil::quit();
}

// Deinitialize The Visual Debugger
void HavokCore::deinitVDB()
{
	_vdb->removeReference();

	// Contexts are not reference counted at the base class level by the vdb as
	// they are just interfaces really.  So only delete the context after you have
	// finished using the visual debugger.
	_context->removeReference();
}

void HavokCore::stepSimulation(float dt)
{
	stepPhysicsSimulation(dt);

	if(_vdbEnabled)
		stepVDBSimulation();

	// Clear accumulated timer data in this thread and all slave threads
	hkMonitorStream::getInstance().reset();
	_threadPool->clearTimerData();
}

void HavokCore::stepPhysicsSimulation(float dt)
{
	// Step the physics world. This single call steps using this thread and all threads in the threadPool.
	// For other products you add jobs, call process all jobs and wait for completion.
	// See the multithreading chapter in the user guide for details.
	_pWorld->stepMultithreaded(_jobQueue, _threadPool, dt);
}

void HavokCore::stepVDBSimulation()
{
	_context->syncTimers(_threadPool);
	_vdb->step();
}

void createPhatomShape()
{

}