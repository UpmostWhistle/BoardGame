#include "Level_Three.h"


Level_Three::Level_Three(void)
{
}


Level_Three::~Level_Three(void)
{
}

// Ball Room Level
void Level_Three::Init( IDirect3DDevice9* m_pD3DDevice, RenderObject* m_pRender, hkpWorld* world )
{
	// initialize the leveled cleared to false
	levelCleared = false;

	// ball room positions
	Ballroom->roomPos = D3DXVECTOR3(-44.0f, 0.0f, 10.0f);
	Ballroom->roomSize = D3DXVECTOR3(23.5, 20.0f, 50.0f);


	// set the constraints for the enemy spawn points
	// low values
	x1 = -60;
	z1 = -35;

	// high values
	x2 = -20;
	z2 = 49;

	// create 2 PURPLE ghosts to appear 
	for ( int i = 0; i < 2; ++i)
	{
		enemies[i] = new Enemy_PurpleGhost();
		enemies[i]->Init( m_pD3DDevice, m_pRender);
		enemies[i]->CreateHavokObject( world );

		float x = RandomBinomial( x1, x2);
		float z = RandomBinomial( z1, z2);

		// set their positions randomly in the BALL ROOM
		enemies[i]->SetPosition( D3DXVECTOR4( x, 0, z, 0));
	}

	// create 1 RED ghost
	enemies[2] = new Enemy_PurpleGhost();
	enemies[2]->Init( m_pD3DDevice, m_pRender);
	enemies[2]->CreateHavokObject( world );

	float x = RandomBinomial( x1, x2);
	float z = RandomBinomial( z1, z2);

	// set their positions randomly in the BALL ROOM
	enemies[2]->SetPosition( D3DXVECTOR4( x, 0, z, 0));

	// create 1 GREEN ghost
	enemies[3] = new Enemy_PurpleGhost();
	enemies[3]->Init( m_pD3DDevice, m_pRender);
	enemies[3]->CreateHavokObject( world );

	x = RandomBinomial( x1, x2);
	z = RandomBinomial( z1, z2);

	// set their positions randomly in the BALL ROOM
	enemies[3]->SetPosition( D3DXVECTOR4( x, 0, z, 0));

	InitRooms();
}

void Level_Three::InitRooms()
{
	Ballroom = new Room();

	hkpRigidBody* rigidBody;
	hkpRigidBodyCinfo bodyInfo;

	enemiesDead = false;
	puzzleSolved = false;
	Ballroom->playerInRoom = false;

	// Box Parameters
	hkVector4 halfExtents(hkVector4(Ballroom->roomSize.x, Ballroom->roomSize.y, Ballroom->roomSize.z, 0.0f));

	// Create Box Based on Parameters
	hkpBoxShape* boxShape = new hkpBoxShape(halfExtents);

	// Set The Object's Properties
	bodyInfo.m_shape = boxShape;
	bodyInfo.m_position.set(Ballroom->roomPos.x, Ballroom->roomPos.y, Ballroom->roomPos.z);

	// Create Rigid Body
	rigidBody = new hkpRigidBody(bodyInfo);

	rigidBody->getCollidable()->getShape()->getAabb(rigidBody->getTransform(), 0.0f, Ballroom->boundingArea);
}