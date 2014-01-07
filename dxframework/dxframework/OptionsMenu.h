#pragma once
#include "BaseMenu.h"
#include "d3dx9Effect.h"

#pragma comment( lib, "D3dx9.lib")

class OptionsMenu : public BaseMenu
{

	 
private:
	////////////////////////////////////////////////////////////////////////
	// Options Menu specific enum
	////////////////////////////////////////////////////////////////////////
	enum OPTIONS_MENU_STATE
	{
		o_OPTIONS_MENU		= 0,
		o_QUIT_TO_MAIN		= 1,
		/*o_FULLSCREEN		= 2,*/
		o_CREDITS			= 2,

	} optionsState;
	bool videoInit;
	
	//////////////////////////////////////////////////////////////////////////
	//// DirectShow COM Object Creation
	//////////////////////////////////////////////////////////////////////////
	//IGraphBuilder			*videoGraph;
	//IMediaControl			*videoControl;
	//IMediaEvent				*videoEvent;
	//IVideoWindow			*videoWindow;
	////HRESULT                *isVideoDone;
	//long evCode;
	//LONG_PTR eventParam1, eventParam2;

	//HWND*					hwnd;
	RECT		mouseSheetRect;

public:
	OptionsMenu(void);
	~OptionsMenu(void);
	 bool videoPlaying;
	bool Init(InputManager* input, IDirect3DDevice9* m_pD3DDevice, HWND wndHandle, D3DPRESENT_PARAMETERS* D3dpp);
	void Update(void);
	void Render();
	void DrawBackground();
	void InitVideo(LPCWSTR vidName);
	void DestroyVideo();
	void OptionsMenu::EnableFullScreen(bool enable);

	// Accessors and Mutators
	int GetState() { return optionsState; };
	void ResetState( int newState ) { optionsState = o_OPTIONS_MENU; };

	bool GetVideoPlaying(){ return videoPlaying; };	
};