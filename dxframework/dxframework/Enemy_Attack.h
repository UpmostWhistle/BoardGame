#pragma once

#include <d3d9.h>
#include <d3dx9.h>

#include "Enemy_Movement.h"
#include "Enemy_Flee.h"
#include "Enemy_Face.h"

class Enemy_Attack
{
private:
	// attack counter variables
	int attackTime;
	float coolDown;

	// velocity variable
	D3DXVECTOR4 velocity;
	float		targetSpeed;
	float		maxSpeed;
	float		maxAcceleration;
	float		TimeToTarget;


	// movement variables
	Enemy_Flee		flee;
	Enemy_Face		face;

	// range variables
	float				fleeRange;
	float				slowRadius;
	float				targetRadius;

public:
	Enemy_Attack(void);
	~Enemy_Attack(void);

	void Update( float dt, Enemy_Movement* movement, D3DXVECTOR4 playerPos );

	// mutators
	void SetSpeed( float newSpeed){ maxSpeed = newSpeed; };
};

