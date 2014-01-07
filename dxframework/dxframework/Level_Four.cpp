#include "Level_Four.h"


Level_Four::Level_Four(void)
{
}


Level_Four::~Level_Four(void)
{
}

// Foyer Level
void Level_Four::Init( IDirect3DDevice9* m_pD3DDevice, RenderObject* m_pRender, hkpWorld* world )
{
	// initialize the leveled cleared to false
	levelCleared = false;

	// foyer posiitons
	Foyer->roomPos = D3DXVECTOR3(1.0f, 0.0f, -14.0f);
	Foyer->roomSize = D3DXVECTOR3(21.5f, 20.0f, 26.0f);


	// set the constraints for the enemy spawn points
	// low values
	x1 = -20;
	z1 = -35;

	// high values
	x2 = 22.5;
	z2 = 4.5;

	// create 1 GREEN ghosts to appear 
	enemies[0] = new Enemy_GreenGhost();
	enemies[0]->Init( m_pD3DDevice, m_pRender);
	enemies[0]->CreateHavokObject( world );

	float x = RandomBinomial( x1, x2);
	float z = RandomBinomial( z1, z2);

	// set their positions randomly in the FOYER
	enemies[0]->SetPosition( D3DXVECTOR4( x, 0, z, 0));

	// create 1 RED ghosts to appear 
	enemies[1] = new Enemy_RedGhost();
	enemies[1]->Init( m_pD3DDevice, m_pRender);
	enemies[1]->CreateHavokObject( world );

	x = RandomBinomial( x1, x2);
	z = RandomBinomial( z1, z2);

	// set their positions randomly in the FOYER
	enemies[1]->SetPosition( D3DXVECTOR4( x, 0, z, 0));

	// create 1 PURPLE ghosts to appear 
	enemies[2] = new Enemy_PurpleGhost();
	enemies[2]->Init( m_pD3DDevice, m_pRender);
	enemies[2]->CreateHavokObject( world );

	x = RandomBinomial( x1, x2);
	z = RandomBinomial( z1, z2);

	// set their positions randomly in the FOYER
	enemies[2]->SetPosition( D3DXVECTOR4( x, 0, z, 0));

	// create 1 YELLOW ghosts to appear 
	enemies[3] = new Enemy_YellowGhost();
	enemies[3]->Init( m_pD3DDevice, m_pRender);
	enemies[3]->CreateHavokObject( world );

	x = RandomBinomial( x1, x2);
	z = RandomBinomial( z1, z2);

	// set their positions randomly in the FOYER
	enemies[3]->SetPosition( D3DXVECTOR4( x, 0, z, 0));
}

void Level_Four::InitRooms()
{
	Foyer = new Room();

	hkpRigidBody* rigidBody;
	hkpRigidBodyCinfo bodyInfo;

	enemiesDead = false;
	puzzleSolved = true;;
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