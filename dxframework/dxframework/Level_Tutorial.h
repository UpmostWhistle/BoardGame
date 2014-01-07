#pragma once
#include "level_base.h"

class Level_Tutorial :	public Level_Base
{
private:
	

public:
	Level_Tutorial(void);
	~Level_Tutorial(void);

	void Init( IDirect3DDevice9* m_pD3DDevice, RenderObject* m_pRender, hkpWorld* world );
	void InitRooms();
	void InitPuzzle( Object_Player* Player, RenderObject* m_pRender, hkpWorld* world);

	void SetEnemyCount( float enemyCount);
};

