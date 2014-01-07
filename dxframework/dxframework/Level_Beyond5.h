#pragma once
#include "level_base.h"
class Level_Beyond5 :	public Level_Base
{
private:
	
	short randColor;

	// constraints for the house, when looking from the imaginary front door.
	float		backWall;
	float		frontWall;
	float		leftWall;
	float		rightWall;

public:
	Level_Beyond5(void);
	~Level_Beyond5(void);

	void Init( IDirect3DDevice9* m_pD3DDevice, RenderObject* m_pRender, hkpWorld* world );
	void InitPuzzle( Object_Player* Player, RenderObject* m_pRender, hkpWorld* world) {};
	void InitRooms() { 0; };
};

