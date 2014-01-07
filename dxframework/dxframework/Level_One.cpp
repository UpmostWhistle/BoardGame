#include "Level_One.h"


Level_One::Level_One(void)
{
}


Level_One::~Level_One(void)
{
}

// Dining Room Level
void Level_One::Init( IDirect3DDevice9* m_pD3DDevice, RenderObject* m_pRender, hkpWorld* world )
{
	// initialize the leveled cleared to false
	levelCleared = false;

	InitRooms();

	// set the constraints for the enemy spawn points
	// low values
	x1 = 22.5;
	z1 = -35;

	// high values
	x2 = 65;
	z2 = 49;

	enemies.resize( 1 );
	// create 4 GREEN ghosts to appear 
	for ( int i = 0; i < 1; ++i)
	{
		enemies[i] = new Enemy_GreenGhost();
		enemies[i]->Init( m_pD3DDevice, m_pRender);
		enemies[i]->CreateHavokObject( world );

		float x = RandomBinomial( x1, x2);
		float z = RandomBinomial( z1, z2);

		// set their positions randomly in the DINING ROOM
		enemies[i]->SetPosition( D3DXVECTOR4( x, 0, z, 0));
	}
}

void Level_One::InitRooms()
{
	Diningroom = new Room();

	// dining Room positions
	Diningroom->roomPos = D3DXVECTOR3(44.0f, 0.0f, 7.5f);
	Diningroom->roomSize = D3DXVECTOR3(21.5, 20.0f, 42.0f);

	hkpRigidBody* rigidBody;
	hkpRigidBodyCinfo bodyInfo;

	enemiesDead = false;
	puzzleSolved = false;
	Diningroom->playerInRoom = false;

	// Box Parameters
	hkVector4 halfExtents(hkVector4(Diningroom->roomSize.x, Diningroom->roomSize.y, Diningroom->roomSize.z, 0.0f));

	// Create Box Based on Parameters
	hkpBoxShape* boxShape = new hkpBoxShape(halfExtents);

	// Set The Object's Properties
	bodyInfo.m_shape = boxShape;
	bodyInfo.m_position.set(Diningroom->roomPos.x, Diningroom->roomPos.y, Diningroom->roomPos.z);

	// Create Rigid Body
	rigidBody = new hkpRigidBody(bodyInfo);

	rigidBody->getCollidable()->getShape()->getAabb(rigidBody->getTransform(), 0.0f, Diningroom->boundingArea);
}