#pragma once
#pragma comment(lib, "winmm.lib")
//////////////////////////////////////////////////////////////////////////
// Direct3D 9 headers and libraries required
//////////////////////////////////////////////////////////////////////////
#include <d3d9.h>
#include <d3dx9.h>
#include <stdio.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dxguid.lib")

// Macro to release COM objects fast and safely
#define SAFE_RELEASE(x) if(x){x->Release(); x = 0;}

enum colorSwitch{g, b, p};
enum numberSwitch{zero, one, two, three, four, five, six, seven, eight, nine, ten, eleven, twelve};

class HUD
{
private:
	//player variables
	float widthHealth;
	float widthShield;

	// health and shield
	float maxHealth;
	float currentHealth;
	
	float maxShield;
	float currentShield;

	//int bullets;

	int level;
	bool hudOn;

	char hudPrint[30];

	int score, highScore;

	// all rects
	//RECT scoreRect, highScoreRect, timeRect, livesRect, levelRect, hudRect, healthRect, shieldRect, ammoRect;

	//exact cuts of the hud items
	RECT hudSheetRect;
	RECT healthSheetRect;
	RECT shieldSheetRect;

	// ammo box sheet rects
	RECT ammoSheetRect;
	RECT bulletSheetRect;
	RECT purpleAmmoSheetRect;
	RECT greenAmmoSheetRect;
	RECT blueAmmoSheetRect;
	RECT numberSheetRect;

	//hud textures
	IDirect3DTexture9* hudTexture;
	IDirect3DTexture9* healthTexture;
	IDirect3DTexture9* shieldTexture;
	
	// stuff for ammo box hud
	IDirect3DTexture9* ammoTexture;
	IDirect3DTexture9* purpleAmmoTexture;
	IDirect3DTexture9* greenAmmoTexture;
	IDirect3DTexture9* blueAmmoTexture;
	IDirect3DTexture9* redAmmoTexture;

	// HUD numbers
	IDirect3DTexture9* zeroTexture;
	IDirect3DTexture9* oneTexture;
	IDirect3DTexture9* twoTexture;
	IDirect3DTexture9* threeTexture;
	IDirect3DTexture9* fourTexture;
	IDirect3DTexture9* fiveTexture;
	IDirect3DTexture9* sixTexture;
	IDirect3DTexture9* sevenTexture;
	IDirect3DTexture9* eightTexture;
	IDirect3DTexture9* nineTexture;
	IDirect3DTexture9* tenTexture;
	IDirect3DTexture9* elevenTexture;
	IDirect3DTexture9* twelveTexture;

	//black bar background
	IDirect3DTexture9* blackBarTexture;
	RECT blackBarSheetRect;


	//enum vars
	colorSwitch color;
	numberSwitch number;

	struct rectPositions
	{
		D3DXVECTOR3 position;
	}
	hudPosition, healthPosition, shieldPosition, ammoPosition, bulletPosition, numberPosition, blackBar1Position, blackBar2Position,
		minimapDotPosition, minimapPosition;

	//hud position variables, the health/shield bars will be dependant on hud pos
	D3DXVECTOR3 hudLocation;

	/************** MINIMAP ***************/
	IDirect3DTexture9*	minimapDotTexture;
	RECT				minimapDotSheetRect;
	IDirect3DTexture9*  minimapBackgroundTexture;
	RECT				minimapSheetRect;
	bool				mapOn;
	/************** END OF MINIMAP *******/

public:
	HUD(void);
	~HUD(void);

	void Init(IDirect3DDevice9* device);

	void Update(float dt, int counter, D3DXVECTOR4 playerPosition);

	void Render(IDirect3DDevice9* device, ID3DXSprite* sprite, int colorSwitch);

	void Shutdown();

	void setColor(colorSwitch color);
	colorSwitch getColor(){return color;}

	void setNumber(numberSwitch number);
	numberSwitch getNumber(){return number;}

	void setHealth(int currentHealth);
	int getHealth(){return currentHealth;}

	void setShield(int currentShield);
	int getShield(){return currentShield;}

	void miniMapOn(bool mapOn);
	bool getMiniMapOn(){return mapOn;}
};

