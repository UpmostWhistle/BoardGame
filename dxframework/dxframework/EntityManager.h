#pragma once

#include <d3dx9.h>
#include <d3d9.h>
#include <vector>
#include "Object_Base.h"
#include "Object_Player.h"

class EntityManager
{
public:

	// Variables
	std::vector<Object_Base*>		v_baseObjects;

	int								numObjects;

	EntityManager(void);
	~EntityManager(void);

	void entityUpdates(float deltaTime, Object_Player* Player);
	void collisionChecks(float deltaTime, Object_Player* Player);

	bool objVsBullet(float deltaTime, Object_Base* object, Object_Player* player);
	bool objVsPlayer(float deltaTime, Object_Base* object, Object_Player* player);
	//bool enemyVsBullet(float deltaTime, Enemy_Base* enemy, Object_Player* player);
	//bool enemyVsPlayer(float deltaTime, Enemy_Base* enemy, Object_Player* player);

	/*void addEnemyObject(Enemy_Base* Enemy);
	void addBaseObject(Object_Base* Object);*/
};

