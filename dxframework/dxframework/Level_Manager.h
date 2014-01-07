#pragma once

#include "Level_Tutorial.h"
#include "Level_One.h"
#include "Level_Two.h"
#include "Level_Three.h"
#include "Level_Four.h"
#include "Level_Beyond5.h"

enum Level{ Tutorial, One, Two, Three, Four, FiveBeyond };

class Level_Manager
{
private:
	// level variabels
	Level_Tutorial*		tutorial;
	Level_One*			levelOne;
	Level_Two*			levelTwo;
	Level_Three*		levelThree;
	Level_Four*			levelFour;
	
	// list of levels after level 4
	std::vector<Level_Beyond5*>		levels5Beyond;
	int								levelCount;

	// device variables
	IDirect3DDevice9*	device;
	RenderObject*		render;
	hkpWorld*			havokWorld;

	// create an Level Enum variable
	Level				level;

	// create a enemy count for the 5+ levels
	float				enemyCount;
public:
	Level_Manager(void);
	~Level_Manager(void);

	void Init(IDirect3DDevice9* m_pD3DDevice, RenderObject* m_pRender, hkpWorld* world, Object_Player* player);

	void Update( float dt, Object_Player* player, gunType bulletColor, D3DXVECTOR3 eyePos, D3DXVECTOR3 lookAt);

	void Render(HWND hwnd, D3DXMATRIX veiwMat, D3DXMATRIX projMat, D3DXVECTOR3 eyePos);
};

