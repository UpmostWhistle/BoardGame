#include "torches.h"
#include "Fire.h"

FourTorchPuzzle::FourTorchPuzzle()
{
	
	D3DXMatrixIdentity(&psysFireWorld);

	psysFireBox.maxPt = D3DXVECTOR3(INFINITY, INFINITY, INFINITY);
	psysFireBox.minPt = D3DXVECTOR3(-INFINITY, -INFINITY, -INFINITY);


	fireSystem1 = new FireRing(L"sprinkler.fx", "SprinklerTech", L"torch.dds", D3DXVECTOR3(0.0f, 0.0f, 0.0f), psysFireBox, 35, 0.0025f);
	fireSystem2 = new FireRing(L"sprinkler.fx", "SprinklerTech", L"torch.dds", D3DXVECTOR3(0.0f, 0.0f, 0.0f), psysFireBox, 35, 0.0025f);
	fireSystem3 = new FireRing(L"sprinkler.fx", "SprinklerTech", L"torch.dds", D3DXVECTOR3(0.0f, 0.0f, 0.0f), psysFireBox, 35, 0.0025f);
	fireSystem4 = new FireRing(L"sprinkler.fx", "SprinklerTech", L"torch.dds", D3DXVECTOR3(0.0f, 0.0f, 0.0f), psysFireBox, 35, 0.0025f);

	fireSystem1->SetType(1);
	fireSystem2->SetType(1);
	fireSystem3->SetType(1);
	fireSystem4->SetType(1);
}


FourTorchPuzzle::~FourTorchPuzzle()
{

}

void FourTorchPuzzle::Init( Object_Player* Player, RenderObject* renderer, hkpWorld* world )
{
	render = renderer;

	fireSystem1 = Player->fireSystem1;
	fireSystem2 = Player->fireSystem2;
	fireSystem3 = Player->fireSystem3;
	fireSystem4 = Player->fireSystem4;

	// Torch
	for(short i = 0; i < ARRAYSIZE(Torches); ++i)
	{
		Torches[i] = new Object_Base();
		Torches[i]->shape = BOX;
		Torches[i]->weight = UNMOVABLE;
		Torches[i]->scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	}

	// torch positions
	Torches[0]->position = D3DXVECTOR4(17.7f, 3.0f, -21.6f, 0.0f); // northeast corner
	Torches[1]->position = D3DXVECTOR4(17.7f, 3.0f, -34.75f, 0.0f); // southeast corner // 23.7
	Torches[2]->position = D3DXVECTOR4(-10.0f, 3.0f, -21.6f, 0.0f); // northwest corner // -16
	Torches[3]->position = D3DXVECTOR4(-10.0f, 3.0f, -34.7f, 0.0f); // southwest corner

	/*Puzzle_FT->fireSystem1->	*/

	Torches[0]->rotation = D3DXVECTOR3(1.6f,0.0f,0.0f); // NE
	Torches[1]->rotation = D3DXVECTOR3(1.6f,0.0f,0.0f); // SE
	Torches[2]->rotation = D3DXVECTOR3(-1.62f,0.0f,0.0f); // NW
	Torches[3]->rotation = D3DXVECTOR3(-1.62f,0.0f,0.0f); // SW

	render->LoadTexture( L"Metal_Texture.jpg", 10);

	for(short i = 0; i < ARRAYSIZE(Torches); ++i)
		render->LoadMesh(L"Torch.X", &Torches[i]->objectMesh);

	for(short i = 0; i < ARRAYSIZE(Torches); ++i)
		Torches[i]->createHavokObject(world);

	for(short i = 0; i < ARRAYSIZE(Torches); ++i)
		Torches[i]->scale = D3DXVECTOR3(5.1050f, 5.1050f, 5.1050f);

	//render->SetScale( D3DXVECTOR4(5.1050f, 5.1050f, 5.1050f, 0.0));
}

void FourTorchPuzzle::Update(float deltaTime, D3DXVECTOR3 eyePos, D3DXVECTOR3 lookAt)
{
	// code for collision checking

	//if( red bullet collides with torch)
	//{ TorchesLit +=1;
	// TorchLit = true; }

	fireSystem1->setPosition(Torches[0]->getPosition());
	fireSystem2->setPosition(Torches[1]->getPosition());
	fireSystem3->setPosition(Torches[2]->getPosition());
	fireSystem4->setPosition(Torches[3]->getPosition());

	// also need to change unlit torch to lit
	fireSystem1->update(deltaTime, eyePos, lookAt);
	fireSystem2->update(deltaTime, eyePos, lookAt);
	fireSystem3->update(deltaTime, eyePos, lookAt);
	fireSystem4->update(deltaTime, eyePos, lookAt);
}

void FourTorchPuzzle::Render(HWND hWnd, D3DXVECTOR3 eyePos, D3DXMATRIX viewMat,D3DXMATRIX projMat)
{

	fireSystem1->draw(hWnd, eyePos, viewMat * projMat);
	fireSystem2->draw(hWnd, eyePos, viewMat * projMat);
	fireSystem3->draw(hWnd, eyePos, viewMat * projMat);
	fireSystem4->draw(hWnd, eyePos, viewMat * projMat);

	for(short i = 0; i < ARRAYSIZE(Torches); ++i)
		render->Render_Object( Torches[i], D3DXVECTOR3(1.0f, -3.0f, 0.3f), viewMat, projMat);
		//render->Render3DObject( Torches[i]->position, Torches[i]->objectMesh, viewMat, projMat, 10);
}

// TODO: add collision stuff
