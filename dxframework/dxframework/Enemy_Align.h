#pragma once

#include "Enemy_Movement.h"

static const double  PI = 3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348;
static const double TWO_PI = 6.2831853071795864769252867665590057683943387987502116419498891846156328125724179972560696;



// holds the time over which to achieve target speed
const float TIME_TO_TARGET = 0.1;

class Enemy_Align
{
protected:
	// holds the max angular acceleration and rotation of enemy
	float maxAngularAcceleration;
	float maxRotation;

	// holds the radius for arriving at the target
	float targetRadius;

	// holds the radius for beginning to slow down
	float slowRadius;

	float MapToRange( float rotation );

public:
	Enemy_Align(void);
	~Enemy_Align(void);

	void GetSteering( Enemy_Movement* movement, float targetOrientation, D3DXVECTOR4 targetPos);
};

