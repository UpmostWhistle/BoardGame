#include "Enemy_Align.h"


Enemy_Align::Enemy_Align(void)
{
	maxAngularAcceleration = 1000.0f;
	maxRotation = 100.0;
	targetRadius = 0.15;
	slowRadius = 0.25;
}

Enemy_Align::~Enemy_Align(void)
{
}

void Enemy_Align::GetSteering( Enemy_Movement* movement, float targetOrientation, D3DXVECTOR4 targetPos)
{
	// get the naive direction to the target
	float rotation = targetOrientation - movement->GetOrientation();

	// map the result to the ( -pi, pi ) interval
	rotation = MapToRange( rotation );
	float rotationSize = std::abs(rotation);

	// check if we are there, if not get steering
	if ( rotationSize >= targetRadius )
	{
		float targetRotation;
		// if we are outside the slowRadius, then use maximum rotation
		if ( rotationSize > slowRadius )
			targetRotation = maxRotation;
		else // calculate a scaled rotation
			targetRotation = maxRotation * rotationSize / slowRadius;

		// the final target rotation combines
		// speed ( already in variable ) and direction;
		targetRotation *= rotation / rotationSize;

		// acceleration tries to get to the target rotation
		movement->SetAngular( targetRotation - movement->GetRotation());
		movement->SetAngular ( movement->GetAngular() / TIME_TO_TARGET);

		// check if the acceleration is too great
		float angularAcceleration = std::abs(movement->GetAngular());

		// if angular accel greater than max normalize and set to max
		if ( angularAcceleration > maxAngularAcceleration)
		{
			movement->SetAngular( movement->GetAngular() / angularAcceleration );
			movement->SetAngular( movement->GetAngular() * maxAngularAcceleration );
		}
		movement->SetLinear( D3DXVECTOR4( 0, 0, 0, 0) );
	}
}

float Enemy_Align::MapToRange( float rotation)
{
	// if the rotation is greater than pi then return the remainder
	if ( rotation >= PI )
	{
		return (int)rotation % (int)PI;
	}
	// else if rotation is less than -pi return the remainder
	else if ( rotation <= -PI )
	{
		return ((int)PI - (int)rotation) % (int)PI;
	}
	// if rotation is in the boundries then return
	else
	{
		return rotation;
	}
}