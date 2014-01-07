#pragma once

#include "Mesh.h"
#include "HavokCore.h"
#include "PSystem.h"

using namespace std;

static const float MAX_HIT_TIMER = 3.0f;

// Enum for deciding what Shape the Object is in Havok
enum HavokPlayerShape
{
	PLAYERNONE			=	0,		// Hav
	PLAYERSPHERE		=	1,		// Havok Sphere
	PLAYERBOX			=	2,		// Havok Box
	PLAYERCAPSULE		=	3		// Havok Capsule
};

enum gunType{green, red, blue};


class Object_Player
{
private:
	AABB psysBox;
	D3DXMATRIX psysWorld;

	void createSphereObject(hkpWorld* world);
	void createBoxObject(hkpWorld* world);
	void createCapsuleObject(hkpWorld* world);
	void stateMachineInit();

public:
	PSystem*	fireSystem1;
	PSystem*	fireSystem2;
	PSystem*	fireSystem3;
	PSystem*	fireSystem4;

struct HavokBullet
{
	D3DXVECTOR3					position;
	D3DXVECTOR3					velocity;
	hkpRigidBody*				bulletObject;
	bool						isAlive;

	void Reset()
	{
		position = D3DXVECTOR3(-1000, 0, 0);
		velocity = D3DXVECTOR3(0, 0, 0);
	}

}bull[12];

	// Base Variables
	D3DXVECTOR4					position;
	D3DXVECTOR3					rotation;
	D3DXVECTOR3					scale;

	// Player Variables
	int							health;
	bool						isAlive;
	bool						wantJump;
	float						jumpTimer;
	bool						beenHit;
	float						hitTimer;
	

	// Physics
	float						velUD;
	float						velLR;

	// Mesh
	Mesh*						objectMesh;
	
	// Havok

	// Rigid Body
	short						shape;
	hkpCharacterRigidBody*		objectBody;
	hkReal						mass;	
	hkVector4					shapeSize;
	hkpCharacterInput			input;
	hkQuaternion				hk_rotation;
	
	// Movement

	// State Machine
	hkpCharacterState*			state;
	hkpCharacterStateManager*	manager;
	hkpCharacterContext*		context;

	// bullet
	PSystem*					mPSys;
	vector<hkpRigidBody*>		bullets;
	vector<D3DXVECTOR3>			bulletPosition;

	// Constructor, Destructor, and Methods
	Object_Player();
	~Object_Player(void);

	void Update(float deltaTime, D3DXVECTOR3 eyePos, D3DXVECTOR3 lookAt, hkpWorld* world);

	void convertPosition();

	hkVector4 velocityCalc(float dt);

	void createHavokObject(hkpWorld* world);

	void createBulletHavokObject(hkpWorld* world, D3DXVECTOR3 bulletPos, short bulletNum);

	void characterInputOutput(D3DXVECTOR3 lookAt, float deltaTime);

	void gunShot();

	void changeGunType(gunType type);

	bool collisionCheck(hkpRigidBody* rigidBody);

	void getBulletPos(hkpWorld* world, float deltaTime);

	void hitInvulTimer(float deltaTime);
};