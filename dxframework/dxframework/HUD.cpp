#include "HUD.h"


HUD::HUD(void)
{
	//enums
	color = g;
	number = twelve;

	//player hud variables
	maxHealth = 100;
	currentHealth = 100;
	
	maxShield = 100;
	currentShield = 100;
	
	//
	//bullets = 20;

	score = 400;
	highScore = 400;

	level = 1;
	hudOn = true;


	//hud pos
	hudLocation = D3DXVECTOR3(0,0,0);

	//rects to reference position
	//SetRect(&scoreRect, 0, 0, 800, 600);
	//SetRect(&highScoreRect, 0, 0, 800, 600);
	//SetRect(&timeRect, 0, 0, 800, 600);
	//SetRect(&livesRect, 0, 0, 800, 600);
	//SetRect(&levelRect, 0, 0, 800, 600);

	//hud
	SetRect(&hudSheetRect, 218, 192, 505, 280);
	hudPosition.position = hudLocation;
	
	//health
	SetRect(&healthSheetRect, 32, 2, 242, 29);
	healthPosition.position.x = hudLocation.x + 37;
	healthPosition.position.y = hudLocation.y + 12;
	healthPosition.position.z = 0.0f;

	//shield
	SetRect(&shieldSheetRect, 32, 2, 242, 29);
	shieldPosition.position.x = hudLocation.x + 37;
	shieldPosition.position.y = hudLocation.y + 40;
	shieldPosition.position.z = 0.0f;
	
	// ammo
	SetRect(&ammoSheetRect, 265, 235, 432, 325);
	ammoPosition.position.x = 0;
	ammoPosition.position.y = 510;
	ammoPosition.position.z = 0.0f;

	//SetRect(&purpleAmmoSheetRect, 297, 217, 342, 262);
	//ammoBulletPosition.position.x = ammoPosition.position.x + 0;
	//ammoBulletPosition.position.y = ammoPosition.position.y + 25;
	//ammoBulletPosition.position.z = ammoPosition.position.z;
	//
	//SetRect(&greenAmmoSheetRect, 297, 217, 342, 262);
	//ammoBulletPosition2.position.x = ammoPosition.position.x + 0;
	//ammoBulletPosition2.position.y = ammoPosition.position.y + 25;
	//ammoBulletPosition2.position.z = ammoPosition.position.z;

	//SetRect(&blueAmmoSheetRect, 297, 217, 342, 262);
	//ammoBulletPosition3.position.x = ammoPosition.position.x + 0;
	//ammoBulletPosition3.position.y = ammoPosition.position.y + 25;
	//ammoBulletPosition3.position.z = ammoPosition.position.z;

	SetRect(&bulletSheetRect, 297, 217, 342, 262);
	bulletPosition.position = D3DXVECTOR3(ammoPosition.position.x, ammoPosition.position.y + 25, ammoPosition.position.z);

	SetRect(&numberSheetRect, 264, 189, 335, 247);
	numberPosition.position.x = 85;
	numberPosition.position.y = 526;
	numberPosition.position.z = 0.0f;

	//var for manip size of bars
	widthHealth = healthSheetRect.right;
	widthShield = shieldSheetRect.right;

	//black bar
	//health black bar
	SetRect(&blackBarSheetRect, 32, 2, 242, 29);
	blackBar1Position.position.x = hudLocation.x + 37;
	blackBar1Position.position.y = hudLocation.y + 12;
	blackBar1Position.position.z = 0.0f;

	//shield black bar
	SetRect(&blackBarSheetRect, 32, 2, 242, 29);
	blackBar2Position.position.x = hudLocation.x + 37;
	blackBar2Position.position.y = hudLocation.y + 40;
	blackBar2Position.position.z = 0.0f;

	/****** minimap *****/
	SetRect(&minimapDotSheetRect, 302, 223, 312, 233);
	
	SetRect(&minimapSheetRect, 320, 205, 575, 375); //260, 180, 445, 330);
	minimapPosition.position = D3DXVECTOR3(564, -21, 0);
	mapOn = true;
	/****** end of minimap****/
}


HUD::~HUD(void)
{
}


void HUD::Init(IDirect3DDevice9* device)
{
	D3DXCreateTextureFromFileExA(device, "blackBar.png", 256, 32, 0, 0,
		D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, 
		D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255), 
		0, 0, &blackBarTexture);

	D3DXCreateTextureFromFileExA(device, "healthHUD.png", 800, 600, 0, 0,
		D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, 
		D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255), 
		0, 0, &hudTexture);
	
	D3DXCreateTextureFromFileExA(device, "LifeBar.png", 256, 32, 0, 0,
		D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, 
		D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255), 
		0, 0, &healthTexture);

	D3DXCreateTextureFromFileExA(device, "ShieldBar.png", 256, 32, 0, 0,
		D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, 
		D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255), 
		0, 0, &shieldTexture);
	
	D3DXCreateTextureFromFileExA(device, "ammoHUD.png", 800, 600, 0, 0,
		D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, 
		D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255), 
		0, 0, &ammoTexture);
		
	D3DXCreateTextureFromFileExA(device, "purpleAmmoHUD.png", 640, 480, 0, 0,
		D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, 
		D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255), 
		0, 0, &purpleAmmoTexture);

	D3DXCreateTextureFromFileExA(device, "redAmmoHUD.png", 640, 480, 0, 0,
		D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, 
		D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255), 
		0, 0, &redAmmoTexture);

	D3DXCreateTextureFromFileExA(device, "greenAmmoHUD.png", 640, 480, 0, 0,
		D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, 
		D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255), 
		0, 0, &greenAmmoTexture);

	D3DXCreateTextureFromFileExA(device, "blueAmmoHUD.png", 640, 480, 0, 0,
		D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, 
		D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255), 
		0, 0, &blueAmmoTexture);

	//number textures
	D3DXCreateTextureFromFileExA(device, "0.png", 640, 480, 0, 0,
		D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, 
		D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255), 
		0, 0, &zeroTexture);
	D3DXCreateTextureFromFileExA(device, "1.png", 640, 480, 0, 0,
		D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, 
		D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255), 
		0, 0, &oneTexture);
	D3DXCreateTextureFromFileExA(device, "2.png", 640, 480, 0, 0,
		D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, 
		D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255), 
		0, 0, &twoTexture);
	D3DXCreateTextureFromFileExA(device, "3.png", 640, 480, 0, 0,
		D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, 
		D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255), 
		0, 0, &threeTexture);
	D3DXCreateTextureFromFileExA(device, "4.png", 640, 480, 0, 0,
		D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, 
		D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255), 
		0, 0, &fourTexture);
	D3DXCreateTextureFromFileExA(device, "5.png", 640, 480, 0, 0,
		D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, 
		D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255), 
		0, 0, &fiveTexture);
	D3DXCreateTextureFromFileExA(device, "6.png", 640, 480, 0, 0,
		D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, 
		D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255), 
		0, 0, &sixTexture);
	D3DXCreateTextureFromFileExA(device, "7.png", 640, 480, 0, 0,
		D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, 
		D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255), 
		0, 0, &sevenTexture);
	D3DXCreateTextureFromFileExA(device, "8.png", 640, 480, 0, 0,
		D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, 
		D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255), 
		0, 0, &eightTexture);
	D3DXCreateTextureFromFileExA(device, "9.png", 640, 480, 0, 0,
		D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, 
		D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255), 
		0, 0, &nineTexture);
	D3DXCreateTextureFromFileExA(device, "10.png", 640, 480, 0, 0,
		D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, 
		D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255), 
		0, 0, &tenTexture);
	D3DXCreateTextureFromFileExA(device, "11.png", 640, 480, 0, 0,
		D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, 
		D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255), 
		0, 0, &elevenTexture);
	D3DXCreateTextureFromFileExA(device, "12.png", 640, 480, 0, 0,
		D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, 
		D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255), 
		0, 0, &twelveTexture);

	D3DXCreateTextureFromFileExA(device, "minimapDot.png", 640, 480, 0, 0,
		D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, 
		D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255), 
		0, 0, &minimapDotTexture);

	D3DXCreateTextureFromFileExA(device, "miniMapBackground3.png", 800, 600, 0, 0,
		D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, 
		D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255), 
		0, 0, &minimapBackgroundTexture);

	//D3DXCreateTextureFromFile(device, "healthHUD.png", &hudTexture);
	hudOn = true;

}

void HUD::Update(float dt, int counter, D3DXVECTOR4 playerPosition)
{
	if(counter == 0)
		number = twelve;
	else if(counter == 1)
		number = eleven;
	else if(counter == 2)
		number = ten;
	else if(counter == 3)
		number = nine;
	else if(counter == 4)
		number = eight;
	else if(counter == 5)
		number = seven;
	else if(counter == 6)
		number = six;
	else if(counter == 7)
		number = five;
	else if(counter == 8)
		number = four;
	else if(counter == 9)
		number = three;
	else if(counter == 10)
		number = two;
	else if(counter == 11)
		number = one;
	else if(counter == 12)
		number = zero;

	minimapDotPosition.position.x = (playerPosition.x * 1.14f) + 685;
	minimapDotPosition.position.y = (-1 * playerPosition.z * 1.0f ) + 79;
	minimapDotPosition.position.z = 0.0f;
}

void HUD::Render(IDirect3DDevice9* device, ID3DXSprite* sprite, int colorSwitch)
{

	if(hudOn)
	{
		/**********************************************************************************************************/
		/**									EQUATIONS FOR REFLECTING SUBTRACTED HEALTH						***/
		/**********************************************************************************************************/
		
		//health
		float remainingHealth = widthHealth - (widthHealth * (  (maxHealth - currentHealth) / maxHealth));

		if(remainingHealth < 32)
			remainingHealth += 32;

		healthSheetRect.right = remainingHealth;

		//shield
		float remainingShield = widthShield - (widthShield * (  (maxShield - currentShield) / maxShield));

		if(remainingShield < 32)
			remainingShield += 32;

		shieldSheetRect.right = remainingShield;
		/**********************************************************************************************************/

	//SetRect(&scoreRect, 0, 0, 800, 600);
	//sprintf_s(hudPrint, "Score: %d", (int)score);
	//font->DrawTextA(0, hudPrint, -1, &scoreRect, 
	//	DT_LEFT | DT_TOP | DT_NOCLIP,
	//	D3DCOLOR_ARGB(255, 255, 255, 255));
	//	
	
	sprite->Draw(blackBarTexture, &blackBarSheetRect, &D3DXVECTOR3(0,0,0), &blackBar1Position.position, D3DCOLOR_ARGB(255, 255, 255, 255));
	sprite->Draw(blackBarTexture, &blackBarSheetRect, &D3DXVECTOR3(0,0,0), &blackBar2Position.position, D3DCOLOR_ARGB(255, 255, 255, 255));	
	sprite->Draw(healthTexture, &healthSheetRect, &D3DXVECTOR3(0,0,0), &healthPosition.position, D3DCOLOR_ARGB(255, 255, 255, 255));
	sprite->Draw(shieldTexture, &shieldSheetRect, &D3DXVECTOR3(0,0,0), &shieldPosition.position, D3DCOLOR_ARGB(255, 255, 255, 255));	
	sprite->Draw(hudTexture, &hudSheetRect, &D3DXVECTOR3(0,0,0), &hudPosition.position, D3DCOLOR_ARGB(255, 255, 255, 255));
	sprite->Draw(ammoTexture, &ammoSheetRect, &D3DXVECTOR3(0,0,0), &ammoPosition.position, D3DCOLOR_ARGB(255, 255, 255, 255));
	switch(color)
	{
	case 0:
		sprite->Draw(greenAmmoTexture, &bulletSheetRect, &D3DXVECTOR3(0,0,0), &bulletPosition.position, D3DCOLOR_ARGB(255, 255, 255, 255));
		break;	
	case 1:
		sprite->Draw(purpleAmmoTexture, &bulletSheetRect, &D3DXVECTOR3(0,0,0), &bulletPosition.position, D3DCOLOR_ARGB(255, 255, 255, 255));
		break;
	case 2:
		sprite->Draw(redAmmoTexture, &bulletSheetRect, &D3DXVECTOR3(0,0,0), &bulletPosition.position, D3DCOLOR_ARGB(255, 255, 255, 255));
		break;
	}

	// not sure if this is a good way to do it but this struct will be in charge of changing the number sprite in the hud
	switch(number)
	{
	case zero:
		sprite->Draw(zeroTexture, &numberSheetRect, &D3DXVECTOR3(0,0,0), &numberPosition.position, D3DCOLOR_ARGB(255,255, 255, 255));
		break;
	case one:
		sprite->Draw(oneTexture, &numberSheetRect, &D3DXVECTOR3(0,0,0), &numberPosition.position, D3DCOLOR_ARGB(255,255, 255, 255));
		break;
	case two:
		sprite->Draw(twoTexture, &numberSheetRect, &D3DXVECTOR3(0,0,0), &numberPosition.position, D3DCOLOR_ARGB(255,255, 255, 255));
		break;
	case three:
		sprite->Draw(threeTexture, &numberSheetRect, &D3DXVECTOR3(0,0,0), &numberPosition.position, D3DCOLOR_ARGB(255,255, 255, 255));
		break;
	case four:
		sprite->Draw(fourTexture, &numberSheetRect, &D3DXVECTOR3(0,0,0), &numberPosition.position, D3DCOLOR_ARGB(255,255, 255, 255));
		break;
	case five:
		sprite->Draw(fiveTexture, &numberSheetRect, &D3DXVECTOR3(0,0,0), &numberPosition.position, D3DCOLOR_ARGB(255,255, 255, 255));
		break;
	case six:
		sprite->Draw(sixTexture, &numberSheetRect, &D3DXVECTOR3(0,0,0), &numberPosition.position, D3DCOLOR_ARGB(255,255, 255, 255));
		break;
	case seven:
		sprite->Draw(sevenTexture, &numberSheetRect, &D3DXVECTOR3(0,0,0), &numberPosition.position, D3DCOLOR_ARGB(255,255, 255, 255));
		break;
	case eight:
		sprite->Draw(eightTexture, &numberSheetRect, &D3DXVECTOR3(0,0,0), &numberPosition.position, D3DCOLOR_ARGB(255,255, 255, 255));
		break;
	case nine:
		sprite->Draw(nineTexture, &numberSheetRect, &D3DXVECTOR3(0,0,0), &numberPosition.position, D3DCOLOR_ARGB(255,255, 255, 255));
		break;
	case ten:
		sprite->Draw(tenTexture, &numberSheetRect, &D3DXVECTOR3(0,0,0), &numberPosition.position, D3DCOLOR_ARGB(255,255, 255, 255));
		break;
	case eleven:
		sprite->Draw(elevenTexture, &numberSheetRect, &D3DXVECTOR3(0,0,0), &numberPosition.position, D3DCOLOR_ARGB(255,255, 255, 255));
		break;
	case twelve:
		sprite->Draw(twelveTexture, &numberSheetRect, &D3DXVECTOR3(0,0,0), &numberPosition.position, D3DCOLOR_ARGB(255,255, 255, 255));
		break;
	}

	//minimap draw
	if(mapOn == true)
	{
	sprite->Draw(minimapBackgroundTexture, &minimapSheetRect, &D3DXVECTOR3(0,0,0), &minimapPosition.position, D3DCOLOR_ARGB(255,255, 255, 255));
	sprite->Draw(minimapDotTexture, &minimapDotSheetRect, &D3DXVECTOR3(0,0,0), &minimapDotPosition.position, D3DCOLOR_ARGB(255,255, 255, 255));
	}

	}
}

void HUD::setColor(colorSwitch color)
{
	this->color = color;
}
void HUD::setHealth(int currentHealth)
{
	this->currentHealth = currentHealth;
}

void HUD::setShield(int currentShield)
{
	this->currentShield = currentShield;
}

void HUD::miniMapOn(bool mapOn)
{
	this->mapOn = mapOn;
}
void HUD::Shutdown()
{
	SAFE_RELEASE(hudTexture)
	SAFE_RELEASE(healthTexture)
	SAFE_RELEASE(shieldTexture)
	SAFE_RELEASE(ammoTexture)
	SAFE_RELEASE(purpleAmmoTexture)
	SAFE_RELEASE(greenAmmoTexture)
	SAFE_RELEASE(blackBarTexture)
	SAFE_RELEASE(minimapDotTexture)
	SAFE_RELEASE(minimapBackgroundTexture)

	SAFE_RELEASE(oneTexture)
	SAFE_RELEASE(twoTexture)
	SAFE_RELEASE(threeTexture)
	SAFE_RELEASE(fourTexture)
	SAFE_RELEASE(fiveTexture)
	SAFE_RELEASE(sixTexture)
	SAFE_RELEASE(sevenTexture)
	SAFE_RELEASE(eightTexture)
	SAFE_RELEASE(nineTexture)
	SAFE_RELEASE(tenTexture)
	SAFE_RELEASE(elevenTexture)
	SAFE_RELEASE(twelveTexture)
}