#pragma once

#include <Windows.h>
#include "InputManager.h"
#include "MenuMain.h"
#include "OptionsMenu.h"
#include "PauseMenu.h"
#include "HUD.h"
#include <dshow.h>
// include sound headers
#include "SoundEffect.h"
#include "SoundLoader.h"
#include "AudioManager.h"



enum GAME_STATE
{
	QUIT			= 0,
	MAIN_MENU		= 1,
	OPTIONS_MENU	= 2,
	GAME			= 3,
	PAUSE_MENU		= 4,
	INTRO			= 5,
};


////////////////////////////////////////////////////////////////////////
/// The GameStateManager will change the state that the game is 
/// currently in to:
///		* Game
///		* Main Menu
///		* Options Menu
///		* Pause Menu
////////////////////////////////////////////////////////////////////////
class GameStateManager
{
private:
	// Needed Variables
	IDirect3DDevice9*		m_pD3DDevice;
	 HWND					hwnd;
	D3DPRESENT_PARAMETERS*	D3Dpp;
	HWND*					hWnd;
	bool*					bWindowed;
	float*					dt;
	HINSTANCE*				hInst;


	// Create variables to classes
	InputManager*			input;
	MenuMain*				mainMenu;
	PauseMenu*				pauseMenu;
	HUD*					hud;
	//Game*					game;
	int					bCounter;
	D3DXVECTOR4			playerPosition;

	////////////////////////////////////////////////////////////////////////
	// DirectShow COM Object Creation
	////////////////////////////////////////////////////////////////////////
	IGraphBuilder			*videoGraph;
	IMediaControl			*videoControl;
	IMediaEvent				*videoEvent;
	IVideoWindow			*videoWindow;
	//HRESULT                *isVideoDone;
	long evCode;
	LONG_PTR eventParam1, eventParam2;
	bool					videoRunning;
	

public:

	OptionsMenu*			optionsMenu;

	// Create a GAME_STATE variable
	GAME_STATE	activeGameState;

	GameStateManager(void);
	~GameStateManager(void);

	void Init(HWND wndHandle,  D3DPRESENT_PARAMETERS* D3dpp, HINSTANCE hInst, IDirect3DDevice9* device);
	void Update( float dt);
	void Render(ID3DXSprite* sprite);

	void onResetDevice();
	void onLostDevice();
	void InitVideo(LPCWSTR vidName);

	void setHudBulletCounter(int bCounter);
	int getHudBulletCounter(){return bCounter;}

	//minimap player position
	void setPlayerPosition(D3DXVECTOR4 playerPosition);
	D3DXVECTOR4 getPlayerPosition(){return playerPosition;}

};

