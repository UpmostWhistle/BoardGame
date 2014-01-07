#include "OptionsMenu.h"


OptionsMenu::OptionsMenu(void)
{
	mousePos.x = 0.0f;
	mousePos.y = 0.0f;
}


OptionsMenu::~OptionsMenu(void)
{
}

bool OptionsMenu::Init(InputManager* input, IDirect3DDevice9* m_pD3DDevice, HWND wndHandle, D3DPRESENT_PARAMETERS* D3dpp)
{
	BaseMenu::Init( input, m_pD3DDevice );
	hwnd = wndHandle;
	myInput = input;
	this->m_pD3DDevice = m_pD3DDevice;
	// create the SPRITE object
	D3DXCreateSprite(m_pD3DDevice, &m_pD3DSprite);

	// create a FONT object
	AddFontResourceEx(L"SanitariumBB.otf", FR_PRIVATE, 0);
	D3DXCreateFont(m_pD3DDevice, 30, 0, FW_BOLD, 0, false, 
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, TEXT("SanitariumBB"), 
		&m_pD3DFont);

	// set the initial selected item
	menuItemSelected = 1;

	// if no image is chosen this will be the default for the background
	backgroundFileName = L"options menu.jpg";

	// Create the background texture
	D3DXCreateTextureFromFileEx(m_pD3DDevice, backgroundFileName ,0,0,0,0,D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, 
		D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255), 
		&m_imageInfo, 0, &backgroundTexture);

		// Create Mouse sprite
	D3DXCreateTextureFromFileEx(m_pD3DDevice, L"cursor.png",0,0,0,0,D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, 
		D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255), 
		&m_cursorInfo, 0, &mouseTexture);
	SetRect(&mouseSheetRect, 7, 4, 24, 30);

	// set back ground position
	backGroundPos = D3DXVECTOR3(0,0,0);

	// Load sound effects
	MenuMusic = SoundLoader::GetInstance()->LoadBGM("MainMenuMusic.ogg");
	AudioManager::GetInstance()->PlayBGM(*MenuMusic);
	AudioManager::GetInstance()->SetBGMVolume(1.0f);

	
	MenuBeep = SoundLoader::GetInstance()->Load(false,false ,"MenuBeep2.mp3");
	AudioManager::GetInstance()->SetSFXVolume(1.0f);
	// sound played counter
	musicPlayCounter = 0;
	mouseCounter = 0;

	videoInit = false;
	videoPlaying = false;
	return true;
}

void OptionsMenu::Update()
{
	BaseMenu::Update();
	myInput->getInput();
	myInput->Update();
	mousePos.x = myInput->GetMousePosX();
	mousePos.y = myInput->GetMousePosY();

	if(mousePos.x >= 290  && mousePos.x < 420 && mousePos.y > 280 && mousePos.y < 300)
	{
			menuItemSelected = 1;
		if( musicPlayCounter < 1)
		AudioManager::GetInstance()->PlaySFX(*MenuBeep);
		musicPlayCounter++;
	}
	
	
	else if(mousePos.x >= 240  && mousePos.x < 465 && mousePos.y > 450 && mousePos.y < 460)
	{
		menuItemSelected = 2;
		if( musicPlayCounter < 1)
		AudioManager::GetInstance()->PlaySFX(*MenuBeep);
		musicPlayCounter++;
	}
	else
		musicPlayCounter = 0;

	
	if ( videoInit == false)
	{
		if (myInput->keyPress( DIK_UP))
		{
			AudioManager::GetInstance()->PlaySFX(*MenuBeep);
			menuItemSelected--;
		}
		else if ( myInput->keyPress( DIK_DOWN))
		{
			AudioManager::GetInstance()->PlaySFX(*MenuBeep);
			menuItemSelected++;
		}
	}

	if ( menuItemSelected < 1)
		menuItemSelected = 2;
	if ( menuItemSelected > 2)
		menuItemSelected = 1;

	if (myInput->keyPress( DIK_ESCAPE))
	{
		PostQuitMessage(0);
	}

	if ( myInput->keyPress(DIK_RETURN) || myInput->isButtonDown(0))
	{
		if ( menuItemSelected == 1)
		{
			optionsState = o_CREDITS;
			
		}
		
		else if ( menuItemSelected == 2)
		{
			optionsState = o_QUIT_TO_MAIN;
		}
	}
	
	if ( optionsState == 2)
	{
		videoPlaying = true;
		InitVideo(L"SplashScreenMovie.wmv");
		videoInit = true;
		videoControl->Run();
	}

	if ( videoInit == true)
	{
		videoEvent->GetEvent(&evCode, &eventParam1, &eventParam2, 0);
		// wait for the video to finish, or wait until the user hits Enter/Return Key
		if(myInput->keyPress( DIK_BACK) || (evCode == EC_COMPLETE))
		{
			
			optionsState = o_OPTIONS_MENU;
			DestroyVideo();
			videoInit = false;
			videoPlaying = false;
			
		}
	}
	else
		myInput->SetMouseDevice(true);

}

void OptionsMenu::Render()
{
	if(videoInit)
		return;
	// Call the base menu's render method to initialize some variables
	BaseMenu::Render();

	m_pD3DSprite->Begin(D3DXSPRITE_ALPHABLEND);
	if(optionsState == 2)
		// // wait for the video to finish, or wait until the user hits Enter/Return Key
		if((myInput->keyDown(DIK_Q)  || (evCode == EC_COMPLETE))) 
		{
			optionsState = o_OPTIONS_MENU;

		}
		DrawBackground();
		// Print Main Menu at the top of the screen
		sprintf(menuPrint,"OPTIONS MENU");
		SetRect(&m_rect,120,130,600,500);  
		option = D3DCOLOR_ARGB(255,150,0,240);

		m_pD3DFont->DrawTextA(0,menuPrint,-1,&m_rect, DT_CENTER | DT_NOCLIP,option);


		sprintf(menuPrint,"Credits");
		SetRect(&m_rect,120,330,600,500);
		if(menuItemSelected == 1)
			option = D3DCOLOR_ARGB(255,255,0,0);
		else
			option = D3DCOLOR_ARGB(255,0,0,255);
		m_pD3DFont->DrawTextA(0,menuPrint,-1,&m_rect, DT_CENTER | DT_NOCLIP,option);

		sprintf(menuPrint,"Exit to Main Menu");
		SetRect(&m_rect,120,530,600,500);
		if(menuItemSelected == 2)
			option = D3DCOLOR_ARGB(255,255,0,0);
		else
			option = D3DCOLOR_ARGB(255,0,0,255);
		m_pD3DFont->DrawTextA(0,menuPrint,-1,&m_rect, DT_CENTER | DT_NOCLIP,option);

		m_pD3DSprite->Draw(mouseTexture, &mouseSheetRect,&D3DXVECTOR3(0,0,0),&D3DXVECTOR3(myInput->GetMousePosX(),myInput->GetMousePosY(),0),D3DCOLOR_ARGB(255, 255, 255, 255));
	
		m_pD3DSprite->End();
}

void OptionsMenu::InitVideo(LPCWSTR vidName)
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
	//GetWindowRect(hwnd, &WinRect);

	// Set the video size to the size of the window

	videoWindow->SetWindowPosition(WinRect.left, WinRect.top, 
		WinRect.right, WinRect.bottom);

	videoWindow->put_Visible(OATRUE);
}

void OptionsMenu::DestroyVideo()
{
	videoControl->Stop();
	videoWindow->put_Visible(OAFALSE);
	SAFE_RELEASE(videoControl);
	SAFE_RELEASE(videoWindow);
	SAFE_RELEASE(videoGraph);
}


void OptionsMenu::DrawBackground()
{
	
	D3DXMATRIX identity;
	D3DXMatrixIdentity(&identity);
	m_pD3DSprite->SetTransform(&identity);
	D3DXMATRIX texScaling;
	D3DXMatrixScaling(&texScaling, 1.f, 1.00f, 0.0f);
	m_pD3DDevice->SetTransform(D3DTS_TEXTURE0, &texScaling);

	D3DXMATRIX T, S;
	D3DXMatrixTranslation(&T,  backGroundPos.x, - backGroundPos.y, - backGroundPos.z);
	D3DXMatrixScaling(&S, 1.0f, 1.2f, 0.0f);
	m_pD3DSprite->SetTransform(&(S*T));

	// Draw the background sprite.
	m_pD3DSprite->Draw(backgroundTexture, 0, 0, 0, D3DCOLOR_XRGB(255, 255, 255));
	m_pD3DSprite->Flush();
	D3DXMatrixScaling(&texScaling, 1.0f, 1.0f, 1.0f);
	m_pD3DDevice->SetTransform(D3DTS_TEXTURE0, &texScaling);
}