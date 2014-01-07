#pragma once

#include "Enemy_Movement.h"
#include "Enemy_Face.h"
#include <cstdlib>

class Enemy_Wander
{
private:
	// kinematic variable
	float maxRotation;

	// radius and foward offset of the wander
	float wanderOffset;
	float wanderRadius;

	// max rate at which the wander orientation can change
	float wanderRate;

	// current orentation of the wander target
	float wanderOrientation;

	// max acceleration of the enemy
	float maxAcceleration;

	Enemy_Face		face;

public:
	Enemy_Wander(void);
	~Enemy_Wander(void);

	void GetSteering( Enemy_Movement* movement);
	void GetKinematicSteering( Enemy_Movement* movement);

	float RandomBinomial();
};

