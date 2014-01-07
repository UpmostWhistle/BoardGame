#pragma once

#include "Enemy_Movement.h"

// the time over which to achieve target speed
const float A_TIME_TO_TARGET = 0.1;

class Enemy_Arrive
{
private:
	// the max accleration and speed of the enemy
	float maxAcceleration;
	float maxSpeed;

	// the radius for arriving at the target
	float targetRadius;

	// the radius for beinning to slow down
	float slowRadius;

public:
	Enemy_Arrive(void);
	~Enemy_Arrive(void);

	void GetSteering( Enemy_Movement* movement, D3DXVECTOR4 targetPos );
};

