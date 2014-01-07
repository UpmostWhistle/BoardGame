#include "Enemy_Flee.h"


Enemy_Flee::Enemy_Flee(void)
{
	// default max speed
	maxSpeed = 1.0f;
}


Enemy_Flee::~Enemy_Flee(void)
{
}

void Enemy_Flee::GetSteering( Enemy_Movement* movement, D3DXVECTOR4 playerPos)
{
	// get the direction from the target
	movement->SetLinear( movement->GetPosition() - playerPos );

	// give full acceleration along this direction
	movement->NormalizeLinear();
	movement->SetLinear( movement->GetLinear() * maxSpeed);

	movement->SetAngular(0);
}
