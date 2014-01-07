#pragma once

#include <d3dx9.h>
#include <d3d9.h>
#include "Object_Base.h"
#include "Object_Player.h"
#include "Enemy_Base.h"
#include "HavokCore.h"

//// Have a max number of enemies for setting a room up;
//static const int FOYER_MAX_ENEMIES;
//static const int BALL_MAX_ENEMIES;
//static const int DINING_MAX_ENEMIES;
//static const int KITCHEN_MAX_ENEMIES;
//
//// Enum for finding the current room
//enum AREA
//{
//	FOYER		= 0,
//	BALLROOM	= 1,
//	DININGROOM	= 2,
//	KITCHEN		= 3,
//	BLAH		= 4,
//};
//
//// A room struct that'll be the base for all the rooms
//struct Room
//{
//	// Bounding Box Area for the Room
//	hkAabb		boundingArea;
//
//	// Room bools
//	bool		puzzleSolved;	// Is that room's puzzle solved?
//	bool		enemiesDead;	// Are all the enemies dead?
//	bool		playerInRoom;	// Is the player currently in the room?
//
//	// room position and scale for the bounding box
//	D3DXVECTOR3	roomPos;
//	D3DXVECTOR3	roomSize;
//
//	// Enemies
//	int			remainingEnemies;
//};

class EventManager
{
public:
	EventManager(void);
	~EventManager(void);

	//// Four Rooms
	//Room		Foyer;
	//Room		Ballroom;
	//Room		Diningroom;
	//Room		Kitchen;

	// Current Room Player Is In
	//AREA		currentRoom;

	void		Init();
	void		Update(float deltaTime);
	void		spawnEnemies();
	void		createRooms();

	// Checking where the player is
	bool		checkForPlayer(Object_Player* Player);
};

