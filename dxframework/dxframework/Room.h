#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include "HavokCore.h"

class Room
{
public:
	// Bounding Box Area for the Room
	hkAabb		boundingArea;

	// Room bools
	bool		playerInRoom;	// Is the player currently in the room?

	// room position and scale for the bounding box
	D3DXVECTOR3	roomPos;
	D3DXVECTOR3	roomSize;

	D3DXVECTOR3 door1Pos;
	D3DXVECTOR3 door2Pos;
};