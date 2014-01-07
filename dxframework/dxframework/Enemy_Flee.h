#pragma once

#include "Enemy_Movement.h"

// The enemy will flee from the player
class Enemy_Flee
{
private:
	// The maxSpeed the enemy can go from the player
	float maxSpeed;
public:
	Enemy_Flee(void);
	~Enemy_Flee(void);

	// algorithm for the flee function
	void GetSteering( Enemy_Movement* movement, D3DXVECTOR4 playerPos);

	// set the maxSpeed per each enemy
	void SetMaxSpeed( float newMaxSpeed ) { maxSpeed = newMaxSpeed; };
};

