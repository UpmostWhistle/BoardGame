#include "Enemy_YellowGhost.h"


Enemy_YellowGhost::Enemy_YellowGhost(void)
{
}


Enemy_YellowGhost::~Enemy_YellowGhost(void)
{
}

void Enemy_YellowGhost::Init(IDirect3DDevice9* m_pD3DDevice, RenderObject* m_pRender)
{
		// call the enemy base init
	Enemy_Base::Init(m_pD3DDevice, m_pRender);



	// initialize the variables
	isDead = false;
	health = 200;
	attackSpeed = 10;
	attackRange = 50;
	wanderRange = 100;

	// Set the Initial movement variables
	movement = new Enemy_Movement();
	movement->setPosition( D3DXVECTOR4(-45.5, 1.0, 2.4, 0));
	movement->setVelocity( D3DXVECTOR4( 0, 0, 0, 0 ));
	movement->SetOrientation( 0.0f);
	movement->SetRotation( 0.0f);
	movement->SetLinear( D3DXVECTOR4(0, 0, 0, 0));
	movement->SetAngular( 0.0f);

	// set the initial state of the enemy
	State = Wander;
	attackInitialized = false;

	// Initialize the 3D model of the enemy.
	textureName = L"YellowGhostTexture.jpg";
	textureNumber = YellowGhost;
	meshName = L"RedGhost.x";

	// Create the texture
	render->LoadTexture( textureName, textureNumber);

	// create 3D mesh from .x file
	render->LoadMesh( meshName, &objectMesh);
}

// check to see if the enemy is hit with a bullet
void Enemy_YellowGhost::BulletCollision( float dt, Object_Player* player, gunType bulletColor)
{
	// collision variables
	hkAabb aabbBase;
	hkAabb aabbOut;


	// Enemy Hits Bullets
	for(short i = 0; i < ARRAYSIZE(player->bull); ++i)
	{
		// Object Hit Bullets
		for(short i = 0; i < ARRAYSIZE(player->bull); ++i)
		{
			// get the aabb bounding box of the ghost
			rigidBody->getCollidable()->getShape()->getAabb(rigidBody->getTransform(), 0.4f, aabbOut);
			// get the aabb bounding box of the player
			player->bull[i].bulletObject->getCollidable()->getShape()->getAabb(player->bull[i].bulletObject->getTransform(), 0.4f, aabbBase);
				
			// if the bullet has collision with the enemy then take appropriate action.
			if(aabbBase.overlaps(aabbOut))
			{
				//// if the bullet is the opposite color as the ghost then lose health
				//if ( bulletColor == purple )
				//	health -= 10;
				//// else if the bullet is the same color then gain health
				//else if ( bulletColor == yellow )
				//	health += 20;
				//// else if other colors then gain health
				//else
				//	health += 10;

				//// if health is greater than 200 set it to a max of 200
				//if ( health > 200 )
				//	health = 200;
			}

		}

	}
}