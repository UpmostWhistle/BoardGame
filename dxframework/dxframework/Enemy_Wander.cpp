#include "Enemy_Wander.h"


Enemy_Wander::Enemy_Wander(void)
{
	// default variables
	maxRotation = 10.0f;

	wanderOffset = 10.0;	// distance between enemy and center of target circle
	wanderRadius = 5.0f;	// radius of circle
	wanderRate = 100.0;		
	maxAcceleration = 200.0f;

	wanderOrientation = 0.0f;
}

Enemy_Wander::~Enemy_Wander(void)
{
}

void Enemy_Wander::GetSteering(Enemy_Movement* movement)
{
	// #1 calculate the target to delegate to face
	// update the wnader orientation
	float randNum = RandomBinomial();
	wanderOrientation += randNum * wanderRate;
	//movement->SetOrientation( movement->GetOrientation() + randNum * wanderRate);

	// calculate the combined target orientation
	float targetOrientation = wanderOrientation + movement->GetOrientation();

	// calculate the center of the wander circle
	D3DXVECTOR4 target = movement->GetPosition() + wanderOffset * movement->GetOrientationAsVector();

	// calculate the target location
	target += wanderRadius * D3DXVECTOR4((float) -sin(targetOrientation), 0, (float) cos(targetOrientation), 0); // targetOrientation.asVector

	// #2 delegate to face
	face.GetSteering(movement, targetOrientation, target);

	// #3 now set the linear acceleration to be at full acceleration in the
	// direction of the orientation
	movement->SetLinear( maxAcceleration * movement->GetOrientationAsVector());
}

void Enemy_Wander::GetKinematicSteering( Enemy_Movement* movement)
{
	// get velocity from the vector form of orientaiton
	movement->setVelocity( MAX_SPEED * movement->GetOrientationAsVector());
	// change our orientation randomly
	float randNum = RandomBinomial();
	movement->SetRotation(randNum * maxRotation);
}

float Enemy_Wander::RandomBinomial()
{
	float random = ((float) rand()) / (float) RAND_MAX;

	float range = 1.0 - (-1.0);  
	return (random*range) + (-1.0);
}