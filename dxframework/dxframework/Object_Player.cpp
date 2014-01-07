#include "Object_Player.h"
#include "Gun.h"
#include "Fire.h"

Object_Player::Object_Player()
{
	objectMesh = new Mesh();
	position = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f);
	scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	rotation = D3DXVECTOR3(0.0f, 0.0f, -0.15f);
	mass = 5.0f;
	shape = PLAYERBOX;

	velUD = 0.0f;
	velLR = 0.0f;

	hitTimer = 0.0f;
	jumpTimer = 0.0f;

	wantJump = false;
	
	// Initialize the particle system.
	D3DXMatrixIdentity(&psysWorld);

	//bullets/gun
	
	psysBox.maxPt = D3DXVECTOR3(INFINITY, INFINITY, INFINITY);
	psysBox.minPt = D3DXVECTOR3(-INFINITY, -INFINITY, -INFINITY);
	
	// Accelerate due to gravity.  However, since the bullets travel at 
	// such a high velocity, the effect of gravity of not really observed.
	gunType type = green;
	changeGunType(type);

	mPSys->setWorldMtx(psysWorld);         

	fireSystem1 = new FireRing(L"sprinkler.fx", "SprinklerTech", L"torch.dds", D3DXVECTOR3(0.0f, 0.0f, 0.0f), psysBox, 35, 0.0025f);
	fireSystem1->SetType(1);
	fireSystem2 = new FireRing(L"sprinkler.fx", "SprinklerTech", L"torch.dds", D3DXVECTOR3(0.0f, 0.0f, 0.0f), psysBox, 35, 0.0025f);
	fireSystem2->SetType(1);
	fireSystem3 = new FireRing(L"sprinkler.fx", "SprinklerTech", L"torch.dds", D3DXVECTOR3(0.0f, 0.0f, 0.0f), psysBox, 35, 0.0025f);
	fireSystem3->SetType(1);
	fireSystem4 = new FireRing(L"sprinkler.fx", "SprinklerTech", L"torch.dds", D3DXVECTOR3(0.0f, 0.0f, 0.0f), psysBox, 35, 0.0025f);
	fireSystem4->SetType(1);
}


Object_Player::~Object_Player(void)
{

}

void Object_Player::Update(float deltaTime, D3DXVECTOR3 eyePos, D3DXVECTOR3 lookAt, hkpWorld* world)
{
	convertPosition();
	characterInputOutput(lookAt, deltaTime);
	getBulletPos(world, deltaTime);

	//gun update
	mPSys->update(deltaTime, eyePos, lookAt);

	hitInvulTimer(deltaTime);

}

void Object_Player::convertPosition()
{
	D3DXVECTOR3 NormRot;
	position.x = (float)objectBody->getPosition().getComponent(0);
	position.y = (float)objectBody->getPosition().getComponent(1);
	position.z = (float)objectBody->getPosition().getComponent(2);
	position.w = (float)objectBody->getPosition().getComponent(3);

	D3DXVec3Normalize(&NormRot, &rotation);

	hk_rotation = hkQuaternion(NormRot.x, NormRot.y, NormRot.z, 0.0f);
	
}

// Changes the velocity in Havok based on velocityUD and velocityLR
hkVector4 Object_Player::velocityCalc(float dt)
{
	hkVector4 tempVel;

	tempVel.set(velLR, 0.0f, velUD, dt);

	return tempVel;
}

// This is a switch that will auto create an Havok Object based on its shape
void Object_Player::createHavokObject(hkpWorld* world)
{
	// Create Object Based on Shape
	switch(shape)
	{
		case PLAYERSPHERE:
			createSphereObject(world);
			stateMachineInit();
			break;

		case PLAYERBOX:
			createBoxObject(world);
			stateMachineInit();
			break;

		case PLAYERCAPSULE:
			createCapsuleObject(world);
			stateMachineInit();
			break;

		case PLAYERNONE:
			createBoxObject(world);
			stateMachineInit();
			break;
	}
}

void Object_Player::createSphereObject(hkpWorld* world)
{
	// Create a temp body info
	hkpCharacterRigidBodyCinfo	bodyInfo;

	// Sphere Parameters
	hkReal radius = (scale.x + scale.z) / 2;

	// Create Sphere Based on Parameters
	hkpSphereShape* sphereShape = new hkpSphereShape(radius);

	// Set The Object's Properties
	bodyInfo.m_shape = sphereShape;
	bodyInfo.m_position.set(position.x, position.y, position.z, 0.0f);
	bodyInfo.m_friction = 1.0f;

	// Calculate Mass Properties
	hkMassProperties massProperties;
	hkpInertiaTensorComputer::computeShapeVolumeMassProperties(sphereShape, mass, massProperties);
	
	// Set Mass Properties

	// Create Rigid Body
	objectBody = new hkpCharacterRigidBody(bodyInfo);

	// No longer need the reference on the shape, as the rigidbody owns it now
	sphereShape->removeReference();

	// Add Rigid Body to the World
	world->addEntity(objectBody->getRigidBody());
}

void Object_Player::createBoxObject(hkpWorld* world)
{
	// Create a temp body info
	hkpCharacterRigidBodyCinfo	bodyInfo;

	// Box Parameters
	hkVector4 halfExtents(scale.x, scale.y, scale.z);

	// Create Box Based on Parameters
	hkpBoxShape* boxShape = new hkpBoxShape(halfExtents);

	// Set The Object's Properties
	bodyInfo.m_shape = boxShape;
	bodyInfo.m_position.set(position.x, position.y, position.z, 0.0f);

	// Calculate Mass Properties
	hkMassProperties massProperties;
	hkpInertiaTensorComputer::computeShapeVolumeMassProperties(boxShape, mass, massProperties);

	// Set Mass Properties
	bodyInfo.m_mass = 100.0f;

	// Create Rigid Body
	objectBody = new hkpCharacterRigidBody(bodyInfo);

	// No longer need the reference on the shape, as the rigidbody owns it now
	boxShape->removeReference();

	// Add Rigid Body to the World
	world->addEntity(objectBody->getRigidBody());
}

void Object_Player::createCapsuleObject(hkpWorld* world)
{
	// Create a temp body info
	hkpCharacterRigidBodyCinfo	bodyInfo;

	// Capsule Parameters
	hkVector4	vertexA(0.0f, 1.0f, 0.0f, 0);	// Top
	hkVector4	vertexB(0.0f, -1.0f, 0.0f, 0);	// Bottom
	hkReal		radius	=	1.0f;				// Radius

	// Create Capsule Based on Parameters
	hkpCapsuleShape* capsuleShape = new hkpCapsuleShape(vertexA, vertexB, radius);

	// Set The Object's Properties
	bodyInfo.m_shape = capsuleShape;
	bodyInfo.m_position.set(position.x, position.y, position.z, 0.0f);
	bodyInfo.m_maxSlope = HK_REAL_PI / 3.0f;

	// Calculate Mass Properties
	hkMassProperties massProperties;

	hkpInertiaTensorComputer::computeShapeVolumeMassProperties(capsuleShape, mass, massProperties);
	
	// Create Rigid Body
	objectBody = new hkpCharacterRigidBody(bodyInfo);

	// No longer need the reference on the shape, as the rigidbody owns it now
	capsuleShape->removeReference();

	// Add Rigid Body to the World
	world->addEntity(objectBody->getRigidBody());
}

void Object_Player::stateMachineInit()
{
	manager = new hkpCharacterStateManager();

	// On the Ground
	state = new hkpCharacterStateOnGround();
	manager->registerState(state, HK_CHARACTER_ON_GROUND);
	state->removeReference();

	// In the Air
	state = new hkpCharacterStateInAir();
	manager->registerState(state, HK_CHARACTER_IN_AIR);
	state->removeReference();

	// Jumping
	state = new hkpCharacterStateJumping();
	manager->registerState(state, HK_CHARACTER_JUMPING);
	state->removeReference();

	// Climbing
	state = new hkpCharacterStateClimbing();
	manager->registerState(state, HK_CHARACTER_CLIMBING);
	state->removeReference();

	context = new hkpCharacterContext(manager, HK_CHARACTER_ON_GROUND);
	manager->removeReference();

	// Set the character type
	context->setCharacterType(hkpCharacterContext::HK_CHARACTER_RIGIDBODY);

}

void Object_Player::characterInputOutput(D3DXVECTOR3 lookAt, float deltaTime)
{
	hkpCharacterInput input;
	hkpCharacterOutput output;

	input.m_inputLR = velLR;
	input.m_inputUD = velUD;

	input.m_wantJump = wantJump;
	input.m_atLadder = false;
	
	input.m_up = hkVector4(0, 1, 0);
	input.m_forward.set(0.0f, 0.0f, D3DXToRadian(rotation.x));
	input.m_forward.setRotatedDir(hk_rotation, input.m_forward);
	
	hkStepInfo stepInfo;
	stepInfo.m_deltaTime = 1.0f / 60.0f;
	stepInfo.m_invDeltaTime = 1.0f / (1.0f / 60.0f);
	input.m_characterGravity.set(0, -16, 0);
	input.m_stepInfo = stepInfo;
	input.m_velocity = objectBody->getRigidBody()->getLinearVelocity();
	input.m_position = objectBody->getRigidBody()->getPosition();

	objectBody->checkSupport(stepInfo, input.m_surfaceInfo);

	context->update(input, output);

	objectBody->setLinearVelocity(output.m_velocity, 1.0f / 60.0f);
}

bool Object_Player::collisionCheck(hkpRigidBody* rigidBody)
{
	hkAabb aabbBase;
	hkAabb aabbOut;

	// Getting both objects' bounding boxes
	rigidBody->getCollidable()->getShape()->getAabb(rigidBody->getTransform(), 0.4f, aabbOut);
	objectBody->getRigidBody()->getCollidable()->getShape()->getAabb(objectBody->getRigidBody()->getTransform(), 0.4f, aabbBase);

	// If there is a collision between the two objects...
	if(aabbBase.overlaps(aabbOut))
	{
		return true;				// ...return true...
	}

	return false;					// ...if not retrun false
}


void Object_Player::changeGunType(gunType type)
{
	switch(type)
	{
		case green:
		mPSys = new Gun(L"gun.fx", "GunTech", L"bolt2.dds", D3DXVECTOR3(0, 0, 0), psysBox, ARRAYSIZE(bull), -1.0f); //gravity changed
		break;

		case red:
		mPSys = new Gun(L"gun.fx", "GunTech", L"bolt4red.dds", D3DXVECTOR3(0, 0, 0), psysBox, ARRAYSIZE(bull), -1.0f); //gravity changed
		break;

		case blue:
		mPSys = new Gun(L"gun.fx", "GunTech", L"bolt3.dds", D3DXVECTOR3(0, 0, 0), psysBox, ARRAYSIZE(bull), -1.0f); //gravity changedd
		break;
	}
	mPSys->setWorldMtx(psysWorld);
}

void Object_Player::createBulletHavokObject(hkpWorld* world, D3DXVECTOR3 bulletPos, short bulletNum)
{
		// Create a temp body info
	hkpRigidBodyCinfo	bodyInfo;

	// Sphere Parameters
	hkReal radius = 1.0f;
	hkReal mass = 20.0f;

	// Create Sphere Based on Parameters
	hkpSphereShape* sphereShape = new hkpSphereShape(radius);

	// Set The Object's Properties
	bodyInfo.m_shape = sphereShape;
	bodyInfo.m_position.set(bulletPos.x, bulletPos.y, bulletPos.z, 0.0f);
	bull[bulletNum].position = bulletPos;
	bodyInfo.m_friction = 1.0f;
	bodyInfo.m_motionType = hkpMotion::MOTION_FIXED;

	// Calculate Mass Properties
	hkMassProperties massProperties;
	hkpInertiaTensorComputer::computeShapeVolumeMassProperties(sphereShape, mass, massProperties);
	
	// Set Mass Properties

	// Create Rigid Body
	bull[bulletNum].bulletObject = new hkpRigidBody(bodyInfo);

	// No longer need the reference on the shape, as the rigidbody owns it now
	sphereShape->removeReference();

	// Add Rigid Body to the World
	world->addEntity(bull[bulletNum].bulletObject);
}

void Object_Player::getBulletPos(hkpWorld* world, float deltaTime)
{
	if(mPSys->GetBulletCounter() <= ARRAYSIZE(bull))
	{
		for(int i = 0; i < mPSys->GetBulletCounter(); i++)
		{
			bull[i].isAlive = true;
			bull[i].position += -1.0f * (bull[i].velocity * 40.0f) * deltaTime + 0.5f * D3DXVECTOR3(0, 0, 0) * deltaTime * deltaTime;
			hkVector4 havokPos = hkVector4(bull[i].position.x, bull[i].position.y + 3.5f, bull[i].position.z, 0.0f);
			bull[i].bulletObject->setPosition(havokPos);
		}
	}
}

void Object_Player::hitInvulTimer(float deltaTime)
{
	if(hitTimer < MAX_HIT_TIMER)
	{
		hitTimer += deltaTime;
		beenHit = true;
	}
	else
	{
		beenHit = false;
	}
}