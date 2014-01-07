#include "Enemy_Face.h"


Enemy_Face::Enemy_Face(void)
{
}


Enemy_Face::~Enemy_Face(void)
{
}

void Enemy_Face::GetSteering( Enemy_Movement* movement, float targetOrientation, D3DXVECTOR4 targetPos)
{
	// #1 calculate the target to delegate to align

	// work out the direction to target
	D3DXVECTOR4 direction = targetPos - movement->GetPosition();

	// check for a zero direction, and make no change if so
	if ( (float) sqrt((direction.x * direction.x) + (direction.z * direction.z)) != 0)
	{
		// put the target together
		targetOrientation = atan2(-direction.x, direction.z);

		//#2 delegate to align
		Enemy_Align::GetSteering( movement, targetOrientation, targetPos);
	}
}