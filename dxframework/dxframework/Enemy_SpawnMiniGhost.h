#pragma once

#include "RenderObject.h"
#include <math.h>

static const double  Pi= 3.14159265;

// struct for creating mini ghosts
struct Ghost_Color
{
	// baby mini variables
	RenderObject*	render;
	bool			isDead;
	short			health;
	D3DXVECTOR4		position;
	LPCWSTR			textureName;
	Mesh*			mesh;
	LPCWSTR			meshName;
	int				textureNum;

	void Init( RenderObject* render, int textureNum)
	{
		this->render = render;
		this->textureNum = textureNum;

		render->LoadTexture( textureName, textureNum);
		render->LoadMesh( meshName, &mesh);

		position = D3DXVECTOR4( 1,-3,1,0);
	};

	void Update( float dt, D3DXVECTOR4 parentPos)
	{
		// moving in a circle around parent ghost
		//float t = dt/ 800.0f;
		//float r = 30.0;
		//position.x *= cosf( t) * r * dt;
		//position.x = sinf( dt ) * r * dt;
		//position.y = -3;
		//position.z = cosf( dt ) * r * dt;

		float radius = 20.0f;
		float speed = 0.0002f;
		float speedScale =  (0.001 * 2 * Pi) / speed;
		float angle = dt * speedScale;

		position.x = (position.x * cos( angle )) - (position.y * sin( angle ));
		position.y = (position.y * cos( angle )) + (position.x * sin( angle ));
		position.z = 0.0f;
		
	};

	void Render( HWND hwnd, D3DXMATRIX veiwMat, D3DXMATRIX projMat)
	{
		render->SetScale( D3DXVECTOR4(0.05, 0.05, 0.05, 0));
		render->Render3DObject( position, mesh, veiwMat, projMat, textureNum);
	};
} ;

class Enemy_SpawnMiniGhost
{
private:
	// Mini Ghosts
	Ghost_Color			purple;
	Ghost_Color			green;
	Ghost_Color			red;
	Ghost_Color			yellow;

public:
	Enemy_SpawnMiniGhost(void);
	~Enemy_SpawnMiniGhost(void);

	bool Init(D3DXVECTOR4 enemyPosition, RenderObject* render, IDirect3DDevice9* device);
	void Update( float dt, D3DXVECTOR4 ghostPosition);
	void Render(HWND hwnd, D3DXMATRIX veiwMat, D3DXMATRIX projMat);
};

