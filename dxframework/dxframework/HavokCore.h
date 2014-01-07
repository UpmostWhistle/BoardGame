#pragma once

#ifndef HAVOK_CORE
#define HAVOK_CORE

// Math and Base Includes
#include <Common/Base/hkBase.h>																	// Havok Base Math Include
#include <Common/Base/Ext/hkBaseExt.h>															// Havok Extended Math Includes
#include <Common/Base/Container/String/hkStringBuf.h>
#include <Common/Base/System/Io/IStream/hkIStream.h>
#include <Common/Base/System/hkBaseSystem.h>
#include <Common/Base/System/Error/hkDefaultError.h>											// Error Checker
#include <Common/Base/Memory/System/hkMemorySystem.h>											// System Memory Includes...
#include <Common/Base/Memory/System/Util/hkMemoryInitUtil.h>									// ...used for finding and setting...
#include <Common/Base/Memory/Allocator/Malloc/hkMallocAllocator.h>								// ...the amount of memory...
#include <Common/Base/Thread/Job/ThreadPool/Cpu/hkCpuJobThreadPool.h>							// ...Havok will use.

#include <Physics/Dynamics/World/hkpWorld.h>													// The Havok World
#include <Physics/Collide/Dispatch/hkpAgentRegisterUtil.h>										// Havok Registration Utility 
#include <Physics/Dynamics/Collide/ContactListener/hkpContactListener.h>

// Visual Debugger Includes
#include <Common/Visualize/hkVisualDebugger.h>													// Visual Debugger Tool
#include <Physics/Utilities/VisualDebugger/hkpPhysicsContext.h>

// Shapes
#include <Physics/Collide/Shape/Convex/Sphere/hkpSphereShape.h>									// Sphere Shape
#include <Physics/Collide/Shape/Convex/Box/hkpBoxShape.h>										// Box Shape
#include <Physics/Collide/Shape/Convex/Capsule/hkpCapsuleShape.h>								// Capsule Shapes
#include <Physics/Collide/Shape/Misc/PhantomCallback/hkpPhantomCallbackShape.h>
#include <Physics/Collide/Shape/Convex/Triangle/hkpTriangleShape.h>
#include <Physics/Dynamics/Entity/hkpRigidBody.h>												// Generic Rigid Body
#include <Physics/Utilities/Dynamics/Inertia/hkpInertiaTensorComputer.h>						// Inertia Physics

// Character Includes
#include <Physics/Utilities/CharacterControl/FirstPersonCharacter/hkpFirstPersonCharacter.h>	// 
#include <Physics/Utilities/CharacterControl/CharacterRigidBody/hkpCharacterRigidBody.h>		// 
#include <Physics/Utilities/CharacterControl/CharacterProxy/hkpCharacterProxy.h>				//
#include <Physics/Utilities/CharacterControl/StateMachine/hkpDefaultCharacterStates.h>			// 


// Display
#include <stdio.h>

class HavokCore
{
private:
	//////////////////////////////////////////////////////////////////////////
	// Variables															//
	//////////////////////////////////////////////////////////////////////////
	bool				_vdbEnabled;				// Visual Debugger
	hkpPhysicsContext*	_context;
	hkVisualDebugger*	_vdb;

	hkJobQueue*			_jobQueue;					
	hkJobThreadPool*	_threadPool;				

	hkpWorld*			_pWorld;					// Physics World
	hkpWorldCinfo		_pWorldInfo;				// Info about global simulation parameters

	//hkgDisplayHandler*	_Display;

	//////////////////////////////////////////////////////////////////////////
	// Inits																//
	//////////////////////////////////////////////////////////////////////////
	void initHavok();								// Initializes Havok
	void initMemory();								// Finds and Sets Memory Upon Init
	void initPhysicsWorld();						// Creates a Physics World
	void initVDB();									// Initializes the Visual Debugger


	//////////////////////////////////////////////////////////////////////////
	// DeInits																//
	//////////////////////////////////////////////////////////////////////////
	void deinitHavok();								// Deinitializes Havok
	void deinitVDB();								// Deinitializes the Visual Debugger
	
	//////////////////////////////////////////////////////////////////////////
	// Updates																//
	//////////////////////////////////////////////////////////////////////////
	void stepPhysicsSimulation(float dt);			// Physics Update
	void stepVDBSimulation();						// Visual Debugger Update

	//////////////////////////////////////////////////////////////////////////
	// Shapes																//
	//////////////////////////////////////////////////////////////////////////
	void createPhatomShape();						// Phantom Shapes for Switches

public:
	HavokCore(bool vdbEnabled);
	~HavokCore(void);

	void stepSimulation(float dt);					// This Combinds the Physics and Visual Debugger Update Methods into One Update

	hkpWorld* getWorld() { return _pWorld; }		// Returns the World
};

#endif