#pragma once
#include "Enemy_Base.h"
class Enemy_PurpleGhost : public Enemy_Base
{
public:
	Enemy_PurpleGhost(void);
	~Enemy_PurpleGhost(void);

	void Init( IDirect3DDevice9* m_pD3DDevice, RenderObject* m_pRender);
	void BulletCollision( float dt, Object_Player* player, gunType bulletColor);
};

