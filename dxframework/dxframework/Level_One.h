#pragma once
#include "level_base.h"
class Level_One :	public Level_Base
{
private:
public:
	Level_One(void);
	~Level_One(void);

	void Init( IDirect3DDevice9* m_pD3DDevice, RenderObject* m_pRender, hkpWorld* world );
	void InitRooms();
	void InitPuzzle( Object_Player* Player, RenderObject* m_pRender, hkpWorld* world) {};
	void SetEnemyCount( float enemyCount);
};

