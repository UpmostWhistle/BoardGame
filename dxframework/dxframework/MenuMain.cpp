#include "MenuMain.h"

MenuMain::MenuMain()
{
	menuState = m_MAIN_MENU;

	mousePos.x = 0.0f;
	mousePos.y = 0.0f;
}

MenuMain::~MenuMain()
{

}


bool MenuMain::Init(InputManager* input, IDirect3DDevice9*	m_pD3DDevice)
{
	// Local pointer to the input manager
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

	D3DXCreateFont(m_pD3DDevice, 110, 0, FW_BOLD, 0, false, 
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, TEXT("SanitariumBB"), 
		&m_pD3DFont2);

	// set the initial selected item
	menuItemSelected = 1;
	
	// if no image is chosen this will be the default for the background
	backgroundFileName = L"haunted_house.jpg";

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

	// Load bgm
	MenuMusic = new SoundEffect();
	MenuMusic = SoundLoader::GetInstance()->LoadBGM("MainMenuMusic.ogg");
	AudioManager::GetInstance()->PlayBGM(*MenuMusic);
	AudioManager::GetInstance()->SetBGMVolume(1.0f);

	// Load sound effects
	MenuBeep = new SoundEffect();
	MenuBeep = SoundLoader::GetInstance()->Load(false,false ,"MenuBeep2.mp3");
	AudioManager::GetInstance()->SetSFXVolume(1.0f);
	// sound played counter
	musicPlayCounter = 0;
	return true;
}

void MenuMain::Update(float dt)
{
	BaseMenu::Update();
	myInput->Update();
	mousePos.x = myInput->GetMousePosX();
	mousePos.y = myInput->GetMousePosY();

	if(mousePos.x >= 280  && mousePos.x < 440 && mousePos.y > 170 && mousePos.y < 200)
	{
		menuItemSelected = 1;
	if( musicPlayCounter < 1)
		AudioManager::GetInstance()->PlaySFX(*MenuBeep);
		musicPlayCounter++;
	}
	
	
	else if(mousePos.x >= 260  && mousePos.x < 440 && mousePos.y > 340 && mousePos.y < 350)
	{
		menuItemSelected = 2;
	if( musicPlayCounter < 1)
		AudioManager::GetInstance()->PlaySFX(*MenuBeep);
		musicPlayCounter++;
	}

	else if(mousePos.x >= 320  && mousePos.x < 400 && mousePos.y > 460 && mousePos.y < 490)
	{
		menuItemSelected = 3;
		if( musicPlayCounter < 1)
		AudioManager::GetInstance()->PlaySFX(*MenuBeep);
		musicPlayCounter++;
	}
	else
		musicPlayCounter = 0;

	if (myInput->keyDown( DIK_RETURN) || myInput->isButtonDown(0))
	{
		if ( menuItemSelected == 1) 
		{
			menuState = m_GAME;
			MenuMusic->Free();
			GameBGM = SoundLoader::GetInstance()->LoadBGM("DST-TheHauntedChapel.mp3");
			AudioManager::GetInstance()->PlayBGM(*GameBGM);
			AudioManager::GetInstance()->SetBGMVolume(5.0f);
		}
		else if ( menuItemSelected == 2)
		{
			menuState = m_OPTIONS_MENU;
		}
		else if ( menuItemSelected == 3)
		{
			menuState = m_QUIT;
		}
	}
}

void MenuMain::Render()
{
	// Call the base menu's render method to initialize some variables
	BaseMenu::Render();
	
	m_pD3DSprite->Begin(D3DXSPRITE_ALPHABLEND);
	DrawBackGround();
	// Print Main Menu at the top of the screen
	sprintf(menuPrint,"Ghost Hunter X");
	SetRect(&m_rect,170,-30,600,500);  
	option = D3DCOLOR_ARGB(255,0,0,240);

	
	m_pD3DFont2->DrawTextA(0,menuPrint,-1,&m_rect, DT_CENTER | DT_NOCLIP,option);

	sprintf(menuPrint,"Game Start");
	SetRect(&m_rect,120,210,600,500);
	if(menuItemSelected == 1)
	 
		option = D3DCOLOR_ARGB(255,255,0,0);
	else
		option = D3DCOLOR_ARGB(255,0,0,255);
	m_pD3DFont->DrawTextA(0,menuPrint,-1,&m_rect, DT_CENTER | DT_NOCLIP,option);

	sprintf(menuPrint,"Options Menu");
	SetRect(&m_rect,120,410,600,500);
	if(menuItemSelected == 2)
		option = D3DCOLOR_ARGB(255,255,0,0);
	else
		option = D3DCOLOR_ARGB(255,0,0,255);
		
	m_pD3DFont->DrawTextA(0,menuPrint,-1,&m_rect, DT_CENTER | DT_NOCLIP,option);

	sprintf(menuPrint,"Quit");
	SetRect(&m_rect,120,550,600,500);
	if(menuItemSelected == 3)
		option = D3DCOLOR_ARGB(255,255,0,0);
	else
		option = D3DCOLOR_ARGB(255,0,0,255);
	m_pD3DFont->DrawTextA(0,menuPrint,-1,&m_rect, DT_CENTER | DT_NOCLIP,option);


	m_pD3DSprite->Draw(mouseTexture, &mouseSheetRect,&D3DXVECTOR3(0,0,0),&D3DXVECTOR3(myInput->GetMousePosX(),myInput->GetMousePosY(),0),D3DCOLOR_ARGB(255, 255, 255, 255));
	
	m_pD3DSprite->End();
}


void MenuMain::DrawBackGround()
{
	
	D3DXMATRIX identity;
	D3DXMatrixIdentity(&identity);
	m_pD3DSprite->SetTransform(&identity);
	D3DXMATRIX texScaling;
	D3DXMatrixScaling(&texScaling, 1.0f, 1.0f, 0.0f);
	m_pD3DDevice->SetTransform(D3DTS_TEXTURE0, &texScaling);

	D3DXMATRIX T, S;
	D3DXMatrixTranslation(&T,  backGroundPos.x,  backGroundPos.y,  backGroundPos.z);
	D3DXMatrixScaling(&S, 1.0f, 1.2f, 0.0f);
	m_pD3DSprite->SetTransform(&(S*T));

	// Draw the background sprite.
	m_pD3DSprite->Draw(backgroundTexture, 0, 0, 0, D3DCOLOR_XRGB(255, 255, 255));
	m_pD3DSprite->Flush();
	D3DXMatrixScaling(&texScaling, 1.0f, 1.0f, 1.0f);
	m_pD3DDevice->SetTransform(D3DTS_TEXTURE0, &texScaling);
}