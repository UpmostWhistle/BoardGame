#include "Enemy_SpawnMiniGhost.h"


Enemy_SpawnMiniGhost::Enemy_SpawnMiniGhost(void)
{
}


Enemy_SpawnMiniGhost::~Enemy_SpawnMiniGhost(void)
{
}

// Initialize the miniGhostClass
bool Enemy_SpawnMiniGhost::Init(D3DXVECTOR4 enemyPosition, RenderObject* render, IDirect3DDevice9* device)
{
	// Initialize the purple enemy
	purple.isDead = false;
	purple.health = 50;
	purple.position = D3DXVECTOR4 ( enemyPosition.x + 10, -2, enemyPosition.z, 0);
	purple.textureName = L"PurpleGhostTexture.JPG";
	purple.meshName = L"RedGhost.x";
	purple.Init( render, 6);

	// initialize the green enemy
	green.isDead = false;
	green.health = 50;
	green.position = D3DXVECTOR4 ( enemyPosition.x - 10, -1, enemyPosition.z, 0);
	green.textureName = L"GreenGhostTexture.JPG";
	green.meshName = L"RedGhost.x";
	green.Init( render, 7);

	// initialize the red enemy
	red.isDead = false;
	red.health = 50;
	red.position = D3DXVECTOR4 ( enemyPosition.x, 0, enemyPosition.z + 10, 0);
	red.textureName = L"RedGhostTexture.JPG";
	red.meshName = L"RedGhost.x";
	red.Init( render, 8);

	// initialize the yellow enemy
	yellow.isDead = false;
	yellow.health = 50;
	yellow.position = D3DXVECTOR4 ( enemyPosition.x, 0, enemyPosition.z - 10, 0);
	yellow.textureName = L"YellowGhostTexture.JPG";
	yellow.meshName = L"RedGhost.x";
	yellow.Init( render, 9);

	return true;
}

// update the position of the enemy
void Enemy_SpawnMiniGhost::Update(float dt, D3DXVECTOR4 ghostPosition)
{
	purple.Update( dt, ghostPosition);
	green.Update( dt, ghostPosition);
	red.Update( dt, ghostPosition);
	yellow.Update( dt, ghostPosition);
}

void Enemy_SpawnMiniGhost::Render(HWND hwnd, D3DXMATRIX veiwMat, D3DXMATRIX projMat)
{
	purple.Render( hwnd, veiwMat, projMat );
	green.Render( hwnd, veiwMat, projMat );
	red.Render( hwnd, veiwMat, projMat );
	yellow.Render( hwnd, veiwMat, projMat );
}