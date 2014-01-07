#include "Enemy_Arrive.h"


Enemy_Arrive::Enemy_Arrive()
{
	// set variables
	maxAcceleration = 20.0f;
	maxSpeed		= 5.0f;
	targetRadius	= 5.0;
	slowRadius		= 10.0f;
}


Enemy_Arrive::~Enemy_Arrive(void)
{
}

void Enemy_Arrive::GetSteering( Enemy_Movement* movement, D3DXVECTOR4 targetPos )
{
	// get the direction to the target
	D3DXVECTOR4 direction = targetPos - movement->GetPosition();
	float distance =  (float) sqrt((direction.x * direction.x) + (direction.z * direction.z));

	// check if we are there, return no steering
	if ( distance >= targetRadius )
	{
		float targetSpeed;

		// if we are outside the slowradius, then go max speed
		if ( distance > slowRadius )
		{
			targetSpeed = maxSpeed;
		}
		else // calculate a scaled speed
		{
			targetSpeed = maxSpeed * distance / slowRadius;
		}

		// the target velocity combines speed and direction
		D3DXVECTOR4 targetVelocity = direction;

		float length = sqrt((targetVelocity.x * targetVelocity.x) + (targetVelocity.z * targetVelocity.z) );
		targetVelocity.x /= length;
		targetVelocity.z /= length;
		targetVelocity *= targetSpeed;

		// acceration tries to get to the target velocity
		movement->SetLinear( targetVelocity - movement->GetVelocity());
		movement->SetLinear( movement->GetLinear() / A_TIME_TO_TARGET );

		// check if the aceleration is too fast
		length = sqrt((movement->GetLinear().x * movement->GetLinear().x) + (movement->GetLinear().z * movement->GetLinear().z) );
		if ( length > maxAcceleration )
		{
			movement->NormalizeLinear();
			movement->SetLinear( movement->GetLinear() * maxAcceleration );
		}

		movement->SetAngular( 0 );
	}

}