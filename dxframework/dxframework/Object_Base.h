#pragma once

#include "Mesh.h"
#include "HavokCore.h"

// Enum for deciding what Shape the Object is in Havok
enum HavokShape
{
	NONE		=	0,		// Havok Box (By Default)
	SPHERE		=	1,		// Havok Sphere
	BOX			=	2,		// Havok Box
	CAPSULE		=	3,		// Havok Capsule
	PHANTOM		=	4		// Havok Phantom
};

enum Moveable
{
	UNMOVABLE	=	0,
	LIGHT		=	1,
	HEAVY		=	2
};



class Object_Base
{
private:
	void createSphereObject(hkpWorld* world);
	void createBoxObject(hkpWorld* world);
	void createCapsuleObject(hkpWorld* world);
	//void stateMachineInit();

public:
	// Variables
	D3DXVECTOR4					position;
	D3DXVECTOR3					rotation;
	D3DXVECTOR3					scale;
	int							health;
	bool						isAlive;
	bool						wantJump;

	// Physics
	float						velUD;
	float						velLR;
	float						jumpTimer;

	// Mesh
	Mesh*						objectMesh;
	
	// Havok

	// Rigid Body
	short						shape;
	hkpRigidBody*				rigidBody;
	hkpRigidBodyCinfo			bodyInfo;
	hkReal						mass;	
	hkVector4					shapeSize;
	hkpCharacterInput			input;
	int							weight;
	
	// Movement

	// State Machine
	hkpCharacterState*			state;
	hkpCharacterStateManager*	manager;
	hkpCharacterContext*		context;

	// Constructor, Destructor, and Methods
	Object_Base(void);
	~Object_Base(void);

	void Update(float deltaTime);

	void convertPosition();

	hkVector4 velocityCalc(float dt);

	void createHavokObject(hkpWorld* world);

	bool collisionCheck(hkpRigidBody* rigidBody); 

	D3DXVECTOR4 getPosition(){return position;}
};

