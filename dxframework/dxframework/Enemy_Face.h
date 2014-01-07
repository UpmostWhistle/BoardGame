#pragma once

#include "Enemy_Align.h"

class Enemy_Face : public Enemy_Align
{
public:
	Enemy_Face(void);
	~Enemy_Face(void);

	void GetSteering( Enemy_Movement* movement, float targetOrientation, D3DXVECTOR4 targetPos);
};

