#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>
#include <windows.h>
#include <iostream>
#include <dshow.h>
#include "InputManager.h"
#include "SoundEffect.h"
#include "SoundLoader.h"
#include "AudioManager.h"

#pragma comment(lib, "Strmiids.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "xinput.lib")

// Macro to release COM objects fast and safely
#define SAFE_RELEASE(x) if(x){x->Release(); x = 0;}

class BaseMenu
{
protected:
	////////////////////////////////////////////////////////////////////////
	// Sound Variables
	////////////////////////////////////////////////////////////////////////
	SoundEffect* MenuBeep;
	SoundEffect* MenuMusic;
	SoundEffect* GameBGM;
	////////////////////////////////////////////////////////////////////////
	// D3D Variables
	////////////////////////////////////////////////////////////////////////
	IDirect3DDevice9*		m_pD3DDevice;		// Device for menus

	//////////////////////////////////////////////////////////////////////////
	// Font Variables
	//////////////////////////////////////////////////////////////////////////
	ID3DXFont*	m_pD3DFont;			// Font Object
	ID3DXFont*	m_pD3DFont2;	
	char		menuPrint[30];		// Holds the text that will be witten on screen
	D3DXCOLOR	option;				// variable to changeduring selection

	//////////////////////////////////////////////////////////////////////////
	// Sprite Variables
	//////////////////////////////////////////////////////////////////////////
	ID3DXSprite*		m_pD3DSprite;		// Sprite Object
	ID3DXSprite*		backgroundSprite;	// Sprite for background
	IDirect3DTexture9*	backgroundTexture;	// Texture to use for background
	IDirect3DTexture9*	mouseTexture;		// texture for mouse cursor
	D3DXIMAGE_INFO		m_imageInfo;		// File details of a texture
	D3DXIMAGE_INFO		m_cursorInfo;		// File Info for the cursor
	RECT				m_rect;				// RECT for the menu background
	LPCWSTR				backgroundFileName;	// name of file for background
	D3DXVECTOR3			backGroundPos;		// position  for background sprite
	RECT				mouseSheetRect;		// for mouse sprite
	int					musicPlayCounter;
	int					mouseCounter;
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
	HWND					hwnd;
	////////////////////////////////////////////////////////////////////////
	// Istance of gameState
	////////////////////////////////////////////////////////////////////////


	int buttonSound;
	int menuItemSelected;		// Hold the menu selection			

	InputManager* myInput;
	D3DXVECTOR2 mousePos;
public:
	BaseMenu(void);
	virtual ~BaseMenu(void);

	virtual bool Init(InputManager* input, IDirect3DDevice9* m_pD3DDevice);
	void Update();
	void Render();

	void onLostDevice();
	void onResetDevice();
};

