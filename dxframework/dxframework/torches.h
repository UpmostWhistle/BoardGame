
#pragma once

#include "Object_Base.h"
#include "EntityManager.h"
#include "PSystem.h"
#include "RenderObject.h"


using namespace std;

class FourTorchPuzzle
{
private:
	// torch variables
	int TorchNumber;
	int TorchesLit;
	bool TorchLit;
	
	// particle variables
	AABB psysFireBox;
	D3DXMATRIX psysFireWorld;

	RenderObject*		render;

public:

	Object_Base* Torches[4];
	PSystem*	fireSystem1;
	PSystem*	fireSystem2; 
	PSystem*	fireSystem3; 
	PSystem*	fireSystem4;

	FourTorchPuzzle();
	~FourTorchPuzzle();

	void Init( Object_Player* Player, RenderObject* render , hkpWorld* world );

	void Update(float deltaTime, D3DXVECTOR3 eyePos, D3DXVECTOR3 lookAt);

	void Render(HWND hWnd, D3DXVECTOR3 eyePos,D3DXMATRIX viewMat, D3DXMATRIX projMat);
};