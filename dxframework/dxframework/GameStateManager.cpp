#include "GameStateManager.h"


GameStateManager::GameStateManager(void)
{
	optionsMenu = NULL;
}


GameStateManager::~GameStateManager(void)
{
}

void GameStateManager::Init( HWND wndHandle,  D3DPRESENT_PARAMETERS* D3dpp, HINSTANCE hInst, IDirect3DDevice9* device)
{
	m_pD3DDevice = device;
	hwnd = wndHandle;
	D3Dpp = D3dpp;

	// Create a new input manager
	input = new InputManager();
	input->init(hInst, hwnd);

	// Create a new menu
	mainMenu = new MenuMain();
	mainMenu->Init( input, m_pD3DDevice );
	
	// intialize video
	InitVideo(L"SplashScreenMovie.wmv");
	
	// Set the active game state 
	activeGameState = MAIN_MENU;
}

void GameStateManager::Update( float dt )
{
	input->getInput();
	
	switch ( activeGameState )
	{
		///////////////////////////////////////////////////////////////////////
	case QUIT:
		{
			// Quit the Application
			activeGameState = QUIT;
			PostQuitMessage(0);
			break;
		}
		///////////////////////////////////////////////////////////////////////
	case MAIN_MENU:
		{
			
			// Call the main menu and return menu selection
			mainMenu->Update(dt);
			input->Update();

			switch ( mainMenu->GetState() )
			{
			case 1:	// Quit
				{
					activeGameState = QUIT;
					break;
				}
			case 2:	// Options Menu
				{
					// delete the main menu
					delete mainMenu;

					// create a new options menu
					optionsMenu = new OptionsMenu();
					optionsMenu->Init( input, m_pD3DDevice, hwnd, D3Dpp);
					// switch the game state to the options menu
					activeGameState = OPTIONS_MENU;
					break;
				}
			case 3:	// Game Start
				{
					// delete the main menu
					delete mainMenu;

					// create a new game
					hud = new HUD();
					hud->Init( m_pD3DDevice);

					activeGameState = GAME;
					break;
				}
			}
			break;
		}
		///////////////////////////////////////////////////////////////////////
	case OPTIONS_MENU:
		{
			// Add optionsMenu update function
			optionsMenu->Update();
			input->Update();
			switch ( optionsMenu->GetState() )
			{
			case 1: // Exit to main menu
				{
					// delete the options menu
					delete optionsMenu;
					optionsMenu = NULL;

					// Create a new menu
					mainMenu = new MenuMain();
					mainMenu->Init( input, m_pD3DDevice );

					// Change the active game state to the main menu
					activeGameState = MAIN_MENU;
					break;
				}
			case 2: // Switch Fullscreen / Windowed mode
				{
					// TODO: Put full screen stuff here

					optionsMenu->ResetState(0);
					break;
				}
			case 3:	// Play credits
				{
					break;
				}
			}
			break;
		}
		///////////////////////////////////////////////////////////////////////
	case GAME:
		{
			// Game's update function
			hud->Update( dt,  getHudBulletCounter(), getPlayerPosition());

			if (input->keyPress(DIK_P))
			{
				// create a pause menu
				pauseMenu = new PauseMenu();
				pauseMenu->Init( input, m_pD3DDevice);

				activeGameState = PAUSE_MENU;
			}
		
			//stuff to update the hud's bullet color
			if (input->keyPress(DIK_1))
			{
				hud->setColor(g);
			}
			if (input->keyPress(DIK_2))
			{
				hud->setColor(b);
			}
			if (input->keyPress(DIK_3))
			{
				hud->setColor(p);	
			}
			if (input->keyPress(DIK_M) || input->keyPress(DIK_TAB))
			{
				if(hud->getMiniMapOn() == true)
				hud->miniMapOn(false);	
				else if(hud->getMiniMapOn() == false)
				hud->miniMapOn(true);	
			}
			break;
		}
		///////////////////////////////////////////////////////////////////////
	case PAUSE_MENU:
		{
			// Pause menu Update
			pauseMenu->Update();
			input->Update();
			switch ( pauseMenu->GetState() )
			{
			case 1: // Resume Game
				{
					// delete the pause menu
					delete pauseMenu;

					activeGameState = GAME;
					break;
				}
			case 2:	// Restart the game
				{
					// delete game and pause menu
					delete hud;
					delete pauseMenu;

					// create a new game
					hud = new HUD();
					hud->Init( m_pD3DDevice);

					activeGameState = GAME;

					break;
				}
			case 3: // Quit to the main menu
				{
					// delete game and pause menu
					delete hud;
					delete pauseMenu;

					// initialize a new main menu
					mainMenu = new MenuMain();
					mainMenu->Init( input, m_pD3DDevice);
					 
					activeGameState = MAIN_MENU;
					break;
				}
			}
			break;
		}
		case INTRO:
		{
			videoControl->Run();
		videoEvent->GetEvent(&evCode, &eventParam1, &eventParam2,0);
		if(input->keyPress(DIK_E) ||  (evCode == EC_COMPLETE))
		{
			activeGameState = MAIN_MENU;
			videoControl->Stop();
			videoWindow->put_Visible(OAFALSE);
			videoWindow->put_Owner((OAHWND)hwnd);
			SAFE_RELEASE(videoControl);
			SAFE_RELEASE(videoEvent);
			SAFE_RELEASE(videoGraph);
		}
		}
		break;
	}
}

void GameStateManager::Render(ID3DXSprite* sprite)
{
	// If the device was not created successfully, return
	if(!m_pD3DDevice)
		return;

	//////////////////////////////////////////////////////////////////////////
	// All draw calls between swap chain's functions, and pre-render and post- 
	// render functions (Clear and Present, BeginScene and EndScene)
	//////////////////////////////////////////////////////////////////////////

	/// Don't need to clear the buffer twice, just once
	/// you are already clearing the buffer in the framework

	//m_pD3DDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DXCOLOR(0.0f, 0.0f, 0.0f, 255.0f), 1.0f, 0);

	// Clear the back buffer, call BeginScene()
	//m_pD3DDevice->BeginScene();

	switch ( activeGameState )
	{
		///////////////////////////////////////////////////////////////////////
	case MAIN_MENU:
		{
			// Render the Main Menu
			mainMenu->Render();
			break;
		}
		///////////////////////////////////////////////////////////////////////
	case GAME:
		{
			// Render the game
			hud->Render(m_pD3DDevice, sprite, hud->getColor());
			break;
		}
		///////////////////////////////////////////////////////////////////////
	case OPTIONS_MENU:
		{
			// Render the Options Menu
			optionsMenu->Render();
			break;
		}
		///////////////////////////////////////////////////////////////////////
	case PAUSE_MENU:
		{
			// Render the Pause menu
			pauseMenu->Render();
			break;
		}
	}

	// EndScene, and Present the back buffer to the display buffer
	//m_pD3DDevice->EndScene();
	//m_pD3DDevice->Present(0, 0, 0, 0);
}

void GameStateManager::onResetDevice()
{
	if ( optionsMenu )
		optionsMenu->onResetDevice();
	else if ( mainMenu )
		mainMenu->onResetDevice();
}

void GameStateManager::onLostDevice()
{
	if ( optionsMenu )
		optionsMenu->onLostDevice();
	else if ( mainMenu )
		mainMenu->onLostDevice();
}


void GameStateManager::InitVideo(LPCWSTR vidName)
{
	CoInitialize(NULL); 

	CoCreateInstance( CLSID_FilterGraph, NULL,
		CLSCTX_INPROC_SERVER, IID_IGraphBuilder,
		(void**)&videoGraph);

	videoGraph->QueryInterface(IID_IMediaControl,
		(void**)&videoControl);

	videoGraph->QueryInterface(IID_IMediaEvent,
		(void**)&videoEvent);

	// building a filter graph for our video
	videoGraph->RenderFile(vidName, NULL);

	//video window
	videoControl->QueryInterface(IID_IVideoWindow,
		(void**)&videoWindow);

	// setup the window
	videoWindow->put_Owner((OAHWND)hwnd);

	// Set the style
	videoWindow->put_WindowStyle(WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE);

	// Obtain the size of the window
	RECT WinRect;
	GetClientRect(hwnd, &WinRect);

	// Set the video size to the size of the window
	videoWindow->SetWindowPosition(WinRect.left, WinRect.top, 
		WinRect.right, WinRect.bottom);
}

void GameStateManager::setHudBulletCounter(int bCounter)
{
	this->bCounter = bCounter;
}

void GameStateManager::setPlayerPosition(D3DXVECTOR4 playerPosition)
{
	this->playerPosition = playerPosition;
}