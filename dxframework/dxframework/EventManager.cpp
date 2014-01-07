#include "EventManager.h"


EventManager::EventManager(void)
{
	//currentRoom = FOYER;
}

EventManager::~EventManager(void)
{

}

void EventManager::Init()
{
	//// Set Rooms Positions, Scale, then create them
	//Foyer.roomPos = D3DXVECTOR3(1.0f, 0.0f, -18.0f);
	//Foyer.roomSize = D3DXVECTOR3(21.5f, 20.0f, 22.0f);
	//
	//
	//Ballroom.roomPos = D3DXVECTOR3(-42.0f, 0.0f, 7.5f);
	//Ballroom.roomSize = D3DXVECTOR3(21.5, 20.0f, 42.0f);
	//
	//
	//Diningroom.roomPos = D3DXVECTOR3(44.0f, 0.0f, 7.5f);
	//Diningroom.roomSize = D3DXVECTOR3(21.5, 20.0f, 42.0f);
	//
	//
	//Kitchen.roomPos = D3DXVECTOR3(1.0f, 0.0f, 27.0f);
	//Kitchen.roomSize = D3DXVECTOR3(21.5f, 20.0f, 22.0f);

	//createRooms();
}

void EventManager::Update(float deltaTime)
{
	// Nothing yet, but just incase
}

void EventManager::createRooms()
{
//	// Create the Foyer
//	{
//		hkpRigidBody* rigidBody;
//		hkpRigidBodyCinfo bodyInfo;
//
//		Foyer.enemiesDead = false;
//		Foyer.puzzleSolved = false;
//		Foyer.playerInRoom = false;
//
//		// Box Parameters
//		hkVector4 halfExtents(hkVector4(Foyer.roomSize.x, Foyer.roomSize.y, Foyer.roomSize.z, 0.0f));
//
//		// Create Box Based on Parameters
//		hkpBoxShape* boxShape = new hkpBoxShape(halfExtents);
//
//		// Set The Object's Properties
//		bodyInfo.m_shape = boxShape;
//		bodyInfo.m_position.set(Foyer.roomPos.x, Foyer.roomPos.y, Foyer.roomPos.z);
//		bodyInfo.m_motionType = hkpMotion::MOTION_FIXED;
//
//		// Create Rigid Body
//		rigidBody = new hkpRigidBody(bodyInfo);
//
//		rigidBody->getCollidable()->getShape()->getAabb(rigidBody->getTransform(), 0.0f, Foyer.boundingArea);
//	}
//
//	// Ballroom
//	{
//		hkpRigidBody* rigidBody;
//		hkpRigidBodyCinfo bodyInfo;
//
//		Ballroom.enemiesDead = false;
//		Ballroom.puzzleSolved = false;
//		Ballroom.playerInRoom = false;
//
//		// Box Parameters
//		hkVector4 halfExtents(hkVector4(Ballroom.roomSize.x, Ballroom.roomSize.y, Ballroom.roomSize.z, 0.0f));
//
//		// Create Box Based on Parameters
//		hkpBoxShape* boxShape = new hkpBoxShape(halfExtents);
//
//		// Set The Object's Properties
//		bodyInfo.m_shape = boxShape;
//		bodyInfo.m_position.set(Ballroom.roomPos.x, Ballroom.roomPos.y, Ballroom.roomPos.z);
//
//		// Create Rigid Body
//		rigidBody = new hkpRigidBody(bodyInfo);
//
//		rigidBody->getCollidable()->getShape()->getAabb(rigidBody->getTransform(), 0.0f, Ballroom.boundingArea);
//	}
//
//	{
//		hkpRigidBody* rigidBody;
//		hkpRigidBodyCinfo bodyInfo;
//
//		Diningroom.enemiesDead = false;
//		Diningroom.puzzleSolved = false;
//		Diningroom.playerInRoom = false;
//
//		// Box Parameters
//		hkVector4 halfExtents(hkVector4(Diningroom.roomSize.x, Diningroom.roomSize.y, Diningroom.roomSize.z, 0.0f));
//
//		// Create Box Based on Parameters
//		hkpBoxShape* boxShape = new hkpBoxShape(halfExtents);
//
//		// Set The Object's Properties
//		bodyInfo.m_shape = boxShape;
//		bodyInfo.m_position.set(Diningroom.roomPos.x, Diningroom.roomPos.y, Diningroom.roomPos.z);
//
//		// Create Rigid Body
//		rigidBody = new hkpRigidBody(bodyInfo);
//
//		rigidBody->getCollidable()->getShape()->getAabb(rigidBody->getTransform(), 0.0f, Diningroom.boundingArea);
//	}
//
//	{
//		hkpRigidBody* rigidBody;
//		hkpRigidBodyCinfo bodyInfo;
//
//		Kitchen.enemiesDead = false;
//		Kitchen.puzzleSolved = false;
//		Kitchen.playerInRoom = false;
//
//		// Box Parameters
//		hkVector4 halfExtents(hkVector4(Kitchen.roomSize.x, Kitchen.roomSize.y, Kitchen.roomSize.z, 0.0f));
//
//		// Create Box Based on Parameters
//		hkpBoxShape* boxShape = new hkpBoxShape(halfExtents);
//
//		// Set The Object's Properties
//		bodyInfo.m_shape = boxShape;
//		bodyInfo.m_position.set(Kitchen.roomPos.x, Kitchen.roomPos.y, Kitchen.roomPos.z);
//
//		// Create Rigid Body
//		rigidBody = new hkpRigidBody(bodyInfo);
//
//		rigidBody->getCollidable()->getShape()->getAabb(rigidBody->getTransform(), 0.0f, Kitchen.boundingArea);
//	}
//	bool candybar;
}

bool EventManager::checkForPlayer(Object_Player* Player)
{
	/*hkAabb player;

	Player->objectBody->getRigidBody()->getCollidable()->getShape()->getAabb(Player->objectBody->getRigidBody()->getTransform(), 0.0f, player);

	if(Foyer.boundingArea.overlaps(player))
	{
		currentRoom = FOYER;
		return true;
	}

	if(Ballroom.boundingArea.overlaps(player))
	{
		currentRoom = BALLROOM;
		return true;
	}


	if(Diningroom.boundingArea.overlaps(player))
	{
		currentRoom = DININGROOM;
		return true;
	}

	if(Kitchen.boundingArea.overlaps(player))
	{
		currentRoom = KITCHEN;
		return true;
	}

	currentRoom = BLAH;
	return false;*/

	return false;
}