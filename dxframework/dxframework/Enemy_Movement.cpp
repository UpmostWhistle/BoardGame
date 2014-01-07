#include "Enemy_Movement.h"


Enemy_Movement::Enemy_Movement(void)
{
	rotation = angular = orientation = 0.0f;
	position = linear = velocity = D3DXVECTOR4(0, 0, 0, 0);
}

Enemy_Movement::~Enemy_Movement(void)
{
}

void Enemy_Movement::Update(float dt, bool isDead)
{
	if(!isDead)
	position.y = 5.0;
	else
	position.y = -20.0;

	velocity.y = 0;

	// update position and orientation
	position += velocity * dt;
	orientation += rotation * dt;

	// and the velocity and rotation
	velocity += linear * dt;
	rotation += angular * dt;

	// check for speeding and clip
	if ( (sqrt((velocity.z * velocity.z) + (velocity.x * velocity.x))) > MAX_SPEED )
	{
		NormalizeVelocity();
		velocity.x *= MAX_SPEED;
		velocity.z *= MAX_SPEED;
	}
}

void Enemy_Movement::GetNewOrientation()
{
	float length = sqrt((velocity.x * velocity.x) + (velocity.z * velocity.z));
	if ( length > 0)
		orientation = (float) atan2(-velocity.x, velocity.z);
}

void Enemy_Movement::NormalizeVelocity()
{
	float length = sqrt((velocity.x * velocity.x) + (velocity.z * velocity.z));
	velocity.x /= length;
	velocity.z /= length;
}
void Enemy_Movement::NormalizeLinear()
{
	float length = sqrt((linear.x * linear.x) + (linear.z * linear.z) );
	linear.x /= length;
	linear.z /= length;
}
/// Accessors and Mutators
float Enemy_Movement::GetOrientation(){	return orientation; }
D3DXVECTOR4 Enemy_Movement::GetOrientationAsVector() 
{	return D3DXVECTOR4((float) -sin(orientation), 0, (float) cos(orientation), 0);}
void Enemy_Movement::SetOrientation(float newOrientation) { orientation = newOrientation ;}
D3DXVECTOR4 Enemy_Movement::GetPosition() { return position;}
void Enemy_Movement::setPosition( D3DXVECTOR4 newPosition) { position = newPosition; }
void Enemy_Movement::setVelocity(D3DXVECTOR4 newVelocity) {velocity = newVelocity; }
D3DXVECTOR4 Enemy_Movement::GetVelocity(){ return velocity; }
float Enemy_Movement::GetVelocityLength() {return (float) sqrt((velocity.x * velocity.x) + (velocity.z * velocity.z));}
D3DXVECTOR4 Enemy_Movement::GetLinear(){ return linear; };
void Enemy_Movement::SetLinear( D3DXVECTOR4 newLinear ){ linear = newLinear; };
float Enemy_Movement::GetRotation(){ return rotation;}
void Enemy_Movement::SetRotation( float newRotation) { rotation = newRotation; }
void Enemy_Movement::SetAngular( float newAngular ) { angular = newAngular; };
float Enemy_Movement::GetAngular() { return angular; };