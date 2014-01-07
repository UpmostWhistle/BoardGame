#pragma once

#include "BaseMenu.h"

class MenuMain : public BaseMenu
{
private:
	////////////////////////////////////////////////////////////////////////
	// Main Menu specific enum
	////////////////////////////////////////////////////////////////////////
	enum MAIN_MENU_STATE
	{
		m_MAIN_MENU		= 0,
		m_QUIT			= 1,
		m_OPTIONS_MENU	= 2,
		m_GAME			= 3,
	} menuState;

	

public:
	MenuMain();
	~MenuMain();
	void DrawBackGround();
	bool Init(InputManager* input, IDirect3DDevice9*	m_pD3DDevice);
	virtual void Render();
	virtual void Update(float dt);

	// Accessors and Mutators
	int GetState() { return menuState; };
};
