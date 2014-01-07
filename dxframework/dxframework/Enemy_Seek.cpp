#include "Enemy_Seek.h"


Enemy_Seek::Enemy_Seek(void)
{
	// default 
	maxSpeed = 0.5f;
	targetSpeed = 0.25f;
	targetRadius = 25.0f;
	slowRadius = 50.0f;
	maxAcceleration = 1.0f;
}


Enemy_Seek::~Enemy_Seek(void)
{
}

void Enemy_Seek::GetSteering( Enemy_Movement* movement, D3DXVECTOR4 playerPos)
{
	// get the direction to the target
	D3DXVECTOR4 direction = playerPos - movement->GetPosition();
	float distance = sqrt(( direction.x * direction.x) + (direction.z + direction.z));

	if ( distance > slowRadius)
	{
		targetSpeed = maxSpeed;
	}
	else
	{
		targetSpeed = maxSpeed * distance / slowRadius;
	}

	D3DXVECTOR4 targetVelocity = direction;
	// normalize targetVelocity
	float length = sqrt((targetVelocity.x * targetVelocity.x) + (targetVelocity.z * targetVelocity.z) + ( targetVelocity.y * targetVelocity.y));
	targetVelocity.x /= length;
	targetVelocity.z /= length;
	targetVelocity.y /= length;
	targetVelocity *= targetSpeed;

	// acceleration tries to get to the target velocity
	movement->SetLinear( targetVelocity - movement->GetVelocity());
	movement->SetLinear( movement->GetLinear() /= TimeToTarget);

	// check if the acceleration is too fast
	if (sqrt(( movement->GetLinear().x * movement->GetLinear().x) + (movement->GetLinear().y * movement->GetLinear().y) + (movement->GetLinear().z * movement->GetLinear().z)) > maxAcceleration)
	{
		movement->NormalizeLinear();
		movement->SetLinear( movement->GetLinear() * maxAcceleration);
	}

	movement->SetAngular(0);
}
