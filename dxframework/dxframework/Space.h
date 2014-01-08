#pragma once

enum SpaceType
{
	BATTLE	= 0,
	CASTLE	= 1,
	TOWN	= 2,
	WEAPON	= 3,
	ITEM	= 4,
	NONE	= 5,
};

enum SpaceRegion
{
	GRASS	= 0,
	DESERT	= 1,
	TUNDRA	= 2,
	WASTE	= 3,
	LAVA	= 4,
	WATER	= 5,
	NONE	= 6,
};

class Space
{
public:
	Space(void);
	~Space(void);
};

