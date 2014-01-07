#include "Level_Tutorial.h"


Level_Tutorial::Level_Tutorial(void)
{
}


Level_Tutorial::~Level_Tutorial(void)
{
	if ( Foyer != NULL)
		delete Foyer;
	if ( Puzzle_FT != NULL)
		delete Puzzle_FT;
}

// Foyer level
void Level_Tutorial::Init( IDirect3DDevice9* m_pD3DDevice, RenderObject* m_pRender, hkpWorld* world)
{
	// initialize the leveled cleared to false
	levelCleared = false;
	enemies.resize( 0 );
	InitRooms();
	Puzzle_FT = new FourTorchPuzzle();
	level = 0;
}

void Level_Tutorial::InitRooms()
{
	Foyer = new Room();

	Foyer->roomPos = D3DXVECTOR3(1.0f, 0.0f, -14.0f);
	Foyer->roomSize = D3DXVECTOR3(21.5f, 20.0f, 26.0f);

	hkpRigidBody* rigidBody;
	hkpRigidBodyCinfo bodyInfo;

	enemiesDead = true;
	puzzleSolved = false;
	Foyer->playerInRoom = false;

	// Box Parameters
	hkVector4 halfExtents(hkVector4(Foyer->roomSize.x, Foyer->roomSize.y, Foyer->roomSize.z, 0.0f));

	// Create Box Based on Parameters
	hkpBoxShape* boxShape = new hkpBoxShape(halfExtents);

	// Set The Object's Properties
	bodyInfo.m_shape = boxShape;
	bodyInfo.m_position.set(Foyer->roomPos.x, Foyer->roomPos.y, Foyer->roomPos.z);
	bodyInfo.m_motionType = hkpMotion::MOTION_FIXED;

	// Create Rigid Body
	rigidBody = new hkpRigidBody(bodyInfo);

	rigidBody->getCollidable()->getShape()->getAabb(rigidBody->getTransform(), 0.0f, Foyer->boundingArea);
}

void Level_Tutorial::InitPuzzle( Object_Player* Player, RenderObject* m_pRender, hkpWorld* world)
{

	Puzzle_FT->Init( Player, m_pRender, world);
}