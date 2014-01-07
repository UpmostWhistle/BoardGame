#pragma once

#include "Enemy_Movement.h"

const float TimeToTarget = 0.1;

class Enemy_Seek
{
private:
	// The maxSpeed the enemy can go to the player
	float maxSpeed;
	float targetSpeed;
	float targetRadius;
	float slowRadius;
	float maxAcceleration;

public:
	Enemy_Seek(void);
	~Enemy_Seek(void);

	// algorithm for the seek function
	void GetSteering( Enemy_Movement* movement, D3DXVECTOR4 playerPos);

	// set the maxSpeed per each enemy
	void SetMaxSpeed( float newMaxSpeed ) { maxSpeed = newMaxSpeed; };
};

