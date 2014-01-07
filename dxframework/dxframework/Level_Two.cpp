#include "Level_Two.h"


Level_Two::Level_Two(void)
{
}


Level_Two::~Level_Two(void)
{
}

// Kitchen Level
void Level_Two::Init( IDirect3DDevice9* m_pD3DDevice, RenderObject* m_pRender, hkpWorld* world )
{
	// initialize the leveled cleared to false
	levelCleared = false;

	// kitchen positions
	Kitchen->roomPos = D3DXVECTOR3(1.0f, 0.0f, 35.0f);
	Kitchen->roomSize = D3DXVECTOR3(21.5f, 20.0f, 26.0f);

	
	// set the constraints for the enemy spawn points
	// low values
	x1 = -20;
	z1 = 4.5;

	// high values
	x2 = 22.5;
	z2 = 49;

	// create 2 GREEN ghosts to appear 
	for ( int i = 0; i < 2; ++i)
	{
		enemies[i] = new Enemy_GreenGhost();
		enemies[i]->Init( m_pD3DDevice, m_pRender);
		enemies[i]->CreateHavokObject( world );


		float x = RandomBinomial( x1, x2);
		float z = RandomBinomial( z1, z2);

		// set their positions randomly in the KITCHEN
		enemies[i]->SetPosition( D3DXVECTOR4( x, 0, z, 0));
	}

	// create 2 RED ghosts to appear 
	for ( int i = 2; i < 4; ++i)
	{
		enemies[i] = new Enemy_RedGhost();
		enemies[i]->Init( m_pD3DDevice, m_pRender);

		float x = RandomBinomial( x1, x2);
		float z = RandomBinomial( z1, z2);

		// set their positions randomly in the KITCHEN
		enemies[i]->SetPosition( D3DXVECTOR4( x, 0, z, 0));
	}

	InitRooms();
}

void Level_Two::InitRooms()
{
	Kitchen = new Room();

	hkpRigidBody* rigidBody;
	hkpRigidBodyCinfo bodyInfo;

	enemiesDead = false;
	puzzleSolved = false;
	Kitchen->playerInRoom = false;

	// Box Parameters
	hkVector4 halfExtents(hkVector4(Kitchen->roomSize.x, Kitchen->roomSize.y, Kitchen->roomSize.z, 0.0f));

	// Create Box Based on Parameters
	hkpBoxShape* boxShape = new hkpBoxShape(halfExtents);

	// Set The Object's Properties
	bodyInfo.m_shape = boxShape;
	bodyInfo.m_position.set(Kitchen->roomPos.x, Kitchen->roomPos.y, Kitchen->roomPos.z);

	// Create Rigid Body
	rigidBody = new hkpRigidBody(bodyInfo);

	rigidBody->getCollidable()->getShape()->getAabb(rigidBody->getTransform(), 0.0f, Kitchen->boundingArea);
}
