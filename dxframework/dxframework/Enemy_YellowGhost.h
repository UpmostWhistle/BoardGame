#pragma once
#include "Enemy_Base.h"
class Enemy_YellowGhost : public Enemy_Base
{
public:
	Enemy_YellowGhost(void);
	~Enemy_YellowGhost(void);

	void Init( IDirect3DDevice9* m_pD3DDevice, RenderObject* m_pRender);
	void BulletCollision( float dt, Object_Player* player, gunType bulletColor);
};

