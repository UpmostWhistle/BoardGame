#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <cmath>

CONST float MAX_SPEED = 2.0;

class Enemy_Movement
{
private:
	// movement variables
	D3DXVECTOR4	position;
	float		orientation;
	D3DXVECTOR4	velocity;
	float		rotation;

	D3DXVECTOR4 linear;
	float		angular;

public:
	Enemy_Movement(void);
	~Enemy_Movement(void);

	void Update( float dt, bool isDead);
	void GetNewOrientation();

	// Accessors and mutators
	float		GetOrientation();
	D3DXVECTOR4 GetOrientationAsVector();
	void		SetOrientation(float newOrientation);

	D3DXVECTOR4 GetPosition();
	void		setPosition( D3DXVECTOR4 newPosition);

	void		setVelocity(D3DXVECTOR4 newVelocity);
	D3DXVECTOR4 GetVelocity();
	float		GetVelocityLength();
	void		NormalizeVelocity();

	float		GetRotation();
	void		SetRotation( float newRotation);

	D3DXVECTOR4 GetLinear();
	void		SetLinear( D3DXVECTOR4 newLinear);
	void		NormalizeLinear();

	void		SetAngular( float newAngular );
	float		GetAngular();
};

