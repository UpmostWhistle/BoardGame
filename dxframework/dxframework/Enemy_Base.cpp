#include "Enemy_Base.h"


// Default variables
Enemy_Base::Enemy_Base(void)
{
	// initialize the variables
	isDead = false;
	health = 200;
	attackSpeed = 10;
	attackRange = 25;
	wanderRange = 75;

	// Hit Delay
	isHit = false;
	hitTimer = 0.0f;

	// set the initial state of the enemy
	State = Wander;

	// Initialize the 3D model of the enemy.
	textureName = L"RedGhostTexture.jpg";
	textureNumber = RedGhost;
	meshName = L"RedGhost.x";

	velUD = 0.0f;
	velLR = 0.0f;

}

// Destructor
Enemy_Base::~Enemy_Base(void)
{
	if ( movement == NULL )
		delete movement;
}

// Initialize all the variables for the enemies
void Enemy_Base::Init(IDirect3DDevice9* m_pD3DDevice, RenderObject* renderObject)
{
	// set external variables to class variables to be used in base class
	device = m_pD3DDevice;
	render = renderObject;

	// havok variables
	velUD = 5.0f;
	velLR = 5.0f;		

	// Create the texture
	render->LoadTexture( textureName, textureNumber);

	// create 3D mesh from .x file
	render->LoadMesh( meshName, &objectMesh);
}

// Update the enemy
void Enemy_Base::Update( float dt, Object_Player* player)
{ 
	this->player = player;

	// if health is less than 0 then enemy is dead
	if ( health <= 0)
		isDead = true;

	// update the state
	UpdateState( State, dt );

	// update the movement and orientation
	movement->GetNewOrientation();
	movement->Update(dt, isDead);

	// update the havok movement
	HavokBodyUpdate();

	// check for player collision
	PlayerCollision( dt );

	// Hit Delay
	hitInvulTimer(dt);
}

// Change the state of the enemy based on current actions in the game
void Enemy_Base::UpdateState(StateType CurrentState, float dt)
{
	switch( CurrentState )
	{
	//	Wander Around Room		/////////////////////////////////////////////////////
	case Wander:
		{
			wander.GetSteering( movement);
			
			// if player is in range then seek player
			if ((sqrt(( player->position.x * player->position.x) + ( player->position.z * player->position.z))) < wanderRange )
				ChangeState( Arrive );
			break;
		}
	//	Seek out player		////////////////////////////////////////////////////////
	case Arrive:
		{
			arrive.GetSteering( movement, player->position);
			// if in attack range of player then attack
			if ((sqrt(( player->position.x * player->position.x) + ( player->position.z * player->position.z))) < attackRange )
			{
				//ChangeState( Attack );
			}

			// if player is out of range then wander
			else if ((sqrt(( player->position.x * player->position.x) + ( player->position.z * player->position.z))) >= wanderRange )
				ChangeState( Wander );
			break;
		}
	//	Flee from player	////////////////////////////////////////////////////////
	case Flee:
		{
			flee.GetSteering( movement, player->position );
			// if out of range of player then wander
			if ((sqrt(( player->position.x * player->position.x) + ( player->position.z * player->position.z))) >= wanderRange )
				ChangeState( Wander );
			break;
		}
	//	Attack player	////////////////////////////////////////////////////////////
	case Attack:
		{
			// set the velocity
			attack.SetSpeed( attackSpeed );
			// update the attack to fire at the player
			attack.Update( dt, movement, player->position);

			// if health is low then flee
			if ( health < 40 )
				ChangeState( Flee );
			break;
		}
	//	If the enemy is close to the wall then seek center of room to adjust  /////
	case PosAdjust:
		{
			arrive.GetSteering( movement, centerOfRoom);

			// if the enemy is close to the center then begin to wander
			if ((sqrt(( player->position.x * player->position.x) + ( player->position.z * player->position.z))) >= 15 )
				ChangeState( Wander );
			break;
		}
	};
}

// render the enemy
void Enemy_Base::Render(HWND hwnd, D3DXMATRIX veiwMat, D3DXMATRIX projMat)
{
	// has to send in orientation to turn properly
	render->SetRotation( movement->GetOrientation());
	//render->SetRotation( movement->GetRotation());

	// set the health based on the scale ( 150 health will = 0.15 scale )
	render->SetScale(D3DXVECTOR4( 0.15, 0.15, 0.15, 0) );

	// render the ghost
	render->Render3DObject( D3DXVECTOR4(movement->GetPosition().x, movement->GetPosition().y - 10.0f, movement->GetPosition().z, movement->GetPosition().w), objectMesh, veiwMat, projMat, textureNumber);
}

// Create 
void Enemy_Base::CreateHavokObject(hkpWorld* world)
{
	// Create a temp body info
	hkpRigidBodyCinfo			bodyInfo;

	// Sphere Parameters
	hkReal radius = 4.0f;

	// Create Sphere Based on Parameters
	hkpSphereShape* sphereShape = new hkpSphereShape(radius);

	// Set The Object's Properties
	bodyInfo.m_shape = sphereShape;
	bodyInfo.m_position.set(movement->GetPosition().x, movement->GetPosition().y, movement->GetPosition().z, 0.0f);
	bodyInfo.m_friction = 1.0f;
	bodyInfo.m_motionType = hkpMotion::MOTION_KEYFRAMED;

	hkReal	mass = 5.0f;
	// Calculate Mass Properties
	hkMassProperties massProperties;
	hkpInertiaTensorComputer::computeShapeVolumeMassProperties(sphereShape, mass, massProperties);
	
	// Set Mass Properties

	// Create Rigid Body
	rigidBody = new hkpRigidBody(bodyInfo);

	// No longer need the reference on the shape, as the rigidbody owns it now
	sphereShape->removeReference();

	// Add Rigid Body to the World
	world->addEntity(rigidBody);
}

// Updates the enemies havok body
void Enemy_Base::HavokBodyUpdate()
{
	// set the havok position to the enemies position
	hkVector4 havokPos = hkVector4(movement->GetPosition().x, movement->GetPosition().y, movement->GetPosition().z, 0.0);
	rigidBody->setPosition(havokPos);
}

// Checks for collision against the player
void Enemy_Base::PlayerCollision( float dt)
{
	// collision variables
	hkAabb aabbBase;
	hkAabb aabbOut;

	// Enemy Hits Player
	if(!player->beenHit)
	{
		// get the aabb bounding box of the ghost
		rigidBody->getCollidable()->getShape()->getAabb(rigidBody->getTransform(), 0.4f, aabbOut);
		// get the aabb bounding box of the player
		player->objectBody->getRigidBody()->getCollidable()->getShape()->getAabb(player->objectBody->getRigidBody()->getTransform(), 0.4f, aabbBase);

		// check the bounding boxes against each other to see if there is collision
		if(aabbBase.overlaps(aabbOut))
		{
			player->health -= 20;
			player->hitTimer = 0.0f;
		}
	}
}

// Collision with the walls
void Enemy_Base::RoomWallCollision( float dt, Room* currentRoom )
{
	hkAabb enemyBoundingBox;

	rigidBody->getCollidable()->getShape()->getAabb( rigidBody->getTransform(), 0.0f, enemyBoundingBox);

	// if the bounding box of the enemy is overlapping the wall bounding box
	if ( enemyBoundingBox.overlaps( currentRoom->boundingArea))
	{
		// find the center of the room
		centerOfRoom.x = (currentRoom->roomSize.x/2) + currentRoom->roomPos.x;
		centerOfRoom.z = ( currentRoom->roomSize.z/2) + currentRoom->roomPos.z;

		// then have the ghost switch to seek out the middle of the room
		ChangeState( PosAdjust );

	}
}

void Enemy_Base::hitInvulTimer(float deltaTime)
{
	if(hitTimer < MAX_HIT_TIMER)
	{
		hitTimer += deltaTime;
		isHit = true;
	}
	else
	{
		isHit = false;
	}
}