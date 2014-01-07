#include "Object_Base.h"


Object_Base::Object_Base(void)
{
	objectMesh = new Mesh();
	position = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f);
	scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	mass = 5.0f;

	shape = BOX;

	velUD = 0.0f;
	velLR = 0.0f;
	health = 100;

	weight = UNMOVABLE;
}


Object_Base::~Object_Base(void)
{
}

void Object_Base::Update(float deltaTime)
{
	convertPosition();
	//characterInputOutput();
}

void Object_Base::convertPosition()
{
	position.x = (float)rigidBody->getPosition().getComponent(0);
	position.y = (float)rigidBody->getPosition().getComponent(1);
	position.z = (float)rigidBody->getPosition().getComponent(2);
	position.w = (float)rigidBody->getPosition().getComponent(3);

	//rotation.x = (float)rigidBody->getRotation().getComponent(0);
}

void Object_Base::createHavokObject(hkpWorld* world)
{
	// Create Object Based on Shape
	switch(shape)
	{
		case SPHERE:
			createSphereObject(world);
			//stateMachineInit();
			break;

		case BOX:
			createBoxObject(world);
			//stateMachineInit();
			break;

		case CAPSULE:
			createCapsuleObject(world);
			//stateMachineInit();
			break;

		case NONE:
			createBoxObject(world);
			//stateMachineInit();
			break;
	}
}

void Object_Base::createSphereObject(hkpWorld* world)
{
	if(weight == LIGHT)
	mass = 5.0f;
	else if(weight == HEAVY)
	mass = 20.0f;

	// Sphere Parameters
	hkReal radius = (scale.x + scale.z) / 2;

	// Create Sphere Based on Parameters
	hkpSphereShape* sphereShape = new hkpSphereShape(radius);

	// Set The Object's Properties
	bodyInfo.m_shape = sphereShape;
	bodyInfo.m_position.set(position.x, position.y, position.z, 0.0f);

	if(weight == LIGHT || weight == HEAVY)
	bodyInfo.m_motionType = hkpMotion::MOTION_DYNAMIC;
	else if(weight == UNMOVABLE)
	bodyInfo.m_motionType = hkpMotion::MOTION_FIXED;

	bodyInfo.m_friction = 1.0f;
	bodyInfo.m_restitution = 0.2f;

	// Calculate Mass Properties
	hkMassProperties massProperties;

	hkpInertiaTensorComputer::computeShapeVolumeMassProperties(sphereShape, mass, massProperties);
	
	// Set Mass Properties
	bodyInfo.setMassProperties(massProperties);

	// Create Rigid Body
	rigidBody = new hkpRigidBody(bodyInfo);

	// No longer need the reference on the shape, as the rigidbody owns it now
	sphereShape->removeReference();

	// Add Rigid Body to the World
	world->addEntity(rigidBody);

}

void Object_Base::createBoxObject(hkpWorld* world)
{
	if(weight == LIGHT)
	mass = 5.0f;
	else if(weight == HEAVY)
	mass = 20.0f;

	// Box Parameters
	hkVector4 halfExtents(scale.x, scale.y, scale.z);

	// Create Box Based on Parameters
	hkpBoxShape* boxShape = new hkpBoxShape(halfExtents);

	// Set The Object's Properties
	bodyInfo.m_shape = boxShape;
	bodyInfo.m_position.set(position.x, position.y, position.z, 0.0f);

	if(weight == LIGHT || weight == HEAVY)
	bodyInfo.m_motionType = hkpMotion::MOTION_DYNAMIC;
	else if(weight == UNMOVABLE)
	bodyInfo.m_motionType = hkpMotion::MOTION_FIXED;

	// Calculate Mass Properties
	hkMassProperties massProperties;
	hkpInertiaTensorComputer::computeShapeVolumeMassProperties(boxShape, mass, massProperties);

	// Set Mass Properties
	bodyInfo.setMassProperties(massProperties);

	// Create Rigid Body
	rigidBody = new hkpRigidBody(bodyInfo);

	// No longer need the reference on the shape, as the rigidbody owns it now
	boxShape->removeReference();

	// Add Rigid Body to the World
	world->addEntity(rigidBody);

}

void Object_Base::createCapsuleObject(hkpWorld* world)
{

	if(weight == LIGHT)
	mass = 5.0f;
	else if(weight == HEAVY)
	mass = 20.0f;

	// Capsule Parameters
	hkVector4	vertexA(position.x, position.y + (scale.y / 2), position.z, 0);	// Top
	hkVector4	vertexB(position.x, position.y - (scale.y / 2), position.z, 0);	// Bottom
	hkReal		radius	=	(scale.x + scale.z) / 2;							// Radius

	// Create Capsule Based on Parameters
	hkpCapsuleShape* capsuleShape = new hkpCapsuleShape(vertexA, vertexB, radius);

	// Set The Object's Properties
	bodyInfo.m_shape = capsuleShape;
	bodyInfo.m_position.set(position.x, position.y, position.z, 0.0f);

	if(weight == LIGHT || weight == HEAVY)
	bodyInfo.m_motionType = hkpMotion::MOTION_DYNAMIC;
	else if(weight == UNMOVABLE)
	bodyInfo.m_motionType = hkpMotion::MOTION_FIXED;


	// Calculate Mass Properties
	hkMassProperties massProperties;

	hkpInertiaTensorComputer::computeShapeVolumeMassProperties(capsuleShape, mass, massProperties);
	
	// Set Mass Properties
	bodyInfo.setMassProperties(massProperties);

	// Create Rigid Body
	rigidBody = new hkpRigidBody(bodyInfo);

	// No longer need the reference on the shape, as the rigidbody owns it now
	capsuleShape->removeReference();

	// Add Rigid Body to the World
	world->addEntity(rigidBody);

}

bool Object_Base::collisionCheck(hkpRigidBody* rigidBody)
{
	hkAabb aabbBase;
	hkAabb aabbOut;

	rigidBody->getCollidable()->getShape()->getAabb(rigidBody->getTransform(), 0.4f, aabbOut);
	this->rigidBody->getCollidable()->getShape()->getAabb(rigidBody->getTransform(), 0.4f, aabbBase);

	if(aabbBase.overlaps(aabbOut))
	{
		return true;
	}

	return false;
}