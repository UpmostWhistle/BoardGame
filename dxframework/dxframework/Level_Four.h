#pragma once
#include "level_base.h"
class Level_Four :	public Level_Base
{
public:
	Level_Four(void);
	~Level_Four(void);

	void Init( IDirect3DDevice9* m_pD3DDevice, RenderObject* m_pRender, hkpWorld* world );
	void InitRooms();
	void InitPuzzle( Object_Player* Player, RenderObject* m_pRender, hkpWorld* world) {};
	void SetEnemyCount( float enemyCount);
};

