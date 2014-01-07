#include "DirectXFramework.h"
#include <crtdbg.h>
//CDirectXFramework* gd3dApp   = 0;
IDirect3DDevice9* m_pD3DDevice = 0;
//CameraObj* gCamera = 0;

CDirectXFramework::CDirectXFramework(void)
{
	// Init or NULL objects before use to avoid any undefined behavior
	m_bVsync		= false;
	m_pD3DObject	= 0; // *handle* naming it gd3dApp for the sake of not having to change more stuff
}

CDirectXFramework::~CDirectXFramework(void)
{
	// If Shutdown is not explicitly called correctly, call it when 
	// this class is destroyed or falls out of scope as an error check.
	Shutdown();
}

void CDirectXFramework::Init(HWND& hWnd, HINSTANCE& hInst, bool bWindowed)
{
	m_hWnd = hWnd;
	
///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Direct3D Foundations - D3D Object, Present Parameters, and D3D Device								 //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Create the D3D Object
	m_pD3DObject = Direct3DCreate9(D3D_SDK_VERSION);

	//RECT
	// Find the width and height of window using hWnd and GetWindowRect()
	RECT windowSizeRect;
	GetWindowRect(hWnd, &windowSizeRect);
	 screenWidth = windowSizeRect.right - windowSizeRect.left;
	 screenHeight = windowSizeRect.bottom - windowSizeRect.top;

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Presentation paramters for creating the D3D9 device													 //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

	
	ZeroMemory(&D3Dpp, sizeof(D3Dpp));						// NULL the structure's memory

	D3Dpp.hDeviceWindow					= hWnd;																		// Handle to the focus window
	D3Dpp.Windowed						= bWindowed;																// Windowed or Full-screen boolean
	D3Dpp.AutoDepthStencilFormat		= D3DFMT_D24S8;																// Format of depth/stencil buffer, 24 bit depth, 8 bit stencil
	D3Dpp.EnableAutoDepthStencil		= TRUE;																		// Enables Z-Buffer (Depth Buffer)
	D3Dpp.BackBufferCount				= 1;																		// Change if need of > 1 is required at a later date
	D3Dpp.BackBufferFormat				= D3DFMT_X8R8G8B8;															// Back-buffer format, 8 bits for each pixel
	D3Dpp.BackBufferHeight				= screenHeight;																// Make sure resolution is supported, use adapter modes
	D3Dpp.BackBufferWidth				= screenWidth;																// (Same as above)
	D3Dpp.SwapEffect					= D3DSWAPEFFECT_DISCARD;													// Discard back-buffer, must stay discard to support multi-sample
	D3Dpp.PresentationInterval			= m_bVsync ? D3DPRESENT_INTERVAL_DEFAULT : D3DPRESENT_INTERVAL_IMMEDIATE;	// Present back-buffer immediately, unless V-Sync is on								
	D3Dpp.Flags							= D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;										// This flag should improve performance, if not set to NULL.
	D3Dpp.FullScreen_RefreshRateInHz	= bWindowed ? 0 : D3DPRESENT_RATE_DEFAULT;									// Full-screen refresh rate, use adapter modes or default
	D3Dpp.MultiSampleQuality			= 0;																		// MSAA currently off, check documentation for support.
	D3Dpp.MultiSampleType				= D3DMULTISAMPLE_NONE;														// MSAA currently off, check documentation for support.

	// Check device capabilities
	DWORD deviceBehaviorFlags = 0;
	m_pD3DObject->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &m_D3DCaps);

	// Determine vertex processing mode
	if(m_D3DCaps.DevCaps & D3DCREATE_HARDWARE_VERTEXPROCESSING)
	{
		// Hardware vertex processing supported? (Video Card)
		deviceBehaviorFlags |= D3DCREATE_HARDWARE_VERTEXPROCESSING;	
	}
	else
	{
		// If not, use software (CPU)
		deviceBehaviorFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING; 
	}

	// If hardware vertex processing is on, check pure device support
	if(m_D3DCaps.DevCaps & D3DDEVCAPS_PUREDEVICE && deviceBehaviorFlags & D3DCREATE_HARDWARE_VERTEXPROCESSING)
	{
		deviceBehaviorFlags |= D3DCREATE_PUREDEVICE;	
	}

	// Create the D3D Device with the present parameters and device flags above
	m_pD3DObject->CreateDevice(D3DADAPTER_DEFAULT,		// which adapter to use, set to primary
								D3DDEVTYPE_HAL,			// device type to use, set to hardware rasterization
								hWnd,					// handle to the focus window
								deviceBehaviorFlags,	// behavior flags
								&D3Dpp,					// presentation parameters
								&m_pD3DDevice);			// returned device pointer

	RECT R;
	GetClientRect(m_hWnd, &R);
	float width2  = (float)R.right;
	float height2 = (float)R.bottom;
	// create a vertex format
	D3DVERTEXELEMENT9 elems[] =
	{
		{0, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 16, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
		{0, 28, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
		D3DDECL_END()
	};
	m_pD3DDevice->CreateVertexDeclaration( elems, &d3dVertexDecl );

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Render																								 //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

	render = new RenderObject();
	render->Init( m_pD3DDevice, m_pD3DSprite);
	//render->CreateVertexElement();
	render->SetVertexDecl( d3dVertexDecl);
	render->MaterialSettings();
	render->LoadShaderEffects( L"TestShader.txt", 0);

	AudioManager::GetInstance()->Initialize();
	// sound
	gunSFX = new SoundEffect();
	gunSFX = SoundLoader::GetInstance()->Load(false,false,"Laser.mp3");
	AudioManager::GetInstance()->SetSFXVolume(1.0f);

	changeBullet = new SoundEffect();
	changeBullet = SoundLoader::GetInstance()->Load(false,false,"gun-cocking-01.wav");
	AudioManager::GetInstance()->SetSFXVolume(1.0f);

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Creating Light																						 //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

	ZeroMemory(&light, sizeof(light));

	light.Type							= D3DLIGHT_DIRECTIONAL;
	light.Ambient						= D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f);
	light.Diffuse						= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	light.Specular						= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	light.Position						= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	light.Range							= 5.0f;
	light.Attenuation0					= 0.0f;
	light.Attenuation1					= 0.05f;
	light.Attenuation2					= 0.0f;

	m_pD3DDevice->SetLight(0, &light);
	m_pD3DDevice->LightEnable(0, true);

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Material Settings																					 //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

	ZeroMemory(&m_pMaterial[0], sizeof(m_pMaterial[0]));
	m_pMaterial[0].Ambient				= D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
	m_pMaterial[0].Diffuse				= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_pMaterial[0].Emissive				= D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_pMaterial[0].Specular				= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_pMaterial[0].Power				= 30.0f;


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Creating Camera																						 //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

	gCamera = new CameraObj();
	gCamera->createCamera(1000.0f, 1.0f, 1.3333f, ((float)screenWidth / (float)screenHeight));
	// Initialize View Matrix
	eyePos								= D3DXVECTOR3(0.0f, 1.0f, -2.0f);	// Camera Position
	lookAt								= eyePos + D3DXVECTOR3(0.0f, 0.0f, 1.0f);	// Position camera is viewing
	upVec								= D3DXVECTOR3(0.0f, 1.0f, 0.0f);	// Rotational orientation

	// Easily calculate the view matrix with 3 intuitive vectors
	D3DXMatrixLookAtLH(&viewMat,											// Returned viewMat
						&eyePos,											// Eye Position
						&lookAt,											// LookAt Position
						&upVec);											// Up Vector

	// Apply the view matrix in the scene
	//m_pD3DDevice->SetTransform(D3DTS_VIEW, &viewMat);

	// Initialize perspective projection matrix, this creates view frustum
	D3DXMatrixPerspectiveFovLH(&projMat,									// Return Projection Matrix
								D3DXToRadian(65.0f),						// Field of View
								(float)screenWidth / (float)screenHeight,	// Aspect Ratio
								1.0f,										// Near Plane
								1000.0f);


	// Apply the projection matrix in the scene
	//m_pD3DDevice->SetTransform(D3DTS_PROJECTION, &projMat);

	m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);

	//camera = new CameraObj();
	//camera->createCamera(5000.0f, 0.01f, 1.3333f, D3DXToRadian(45.0f));


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Loading Textures																						 //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

	D3DXCreateSprite(m_pD3DDevice, &m_pD3DSprite);

	// create a TEXTURE object
	D3DXCreateTextureFromFileEx(m_pD3DDevice, L"Metal_Texture.jpg", 0, 0, 0, 0,
		D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, 
		D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255), 
		&m_imageInfo, 0, &m_pTexture[0]);

	D3DXCreateTextureFromFileEx(m_pD3DDevice, L"test.tga", 0, 0, 0, 0,
		D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, 
		D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255), 
		&m_imageInfo, 0, &m_pTexture[1]);

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Create Font COM Object																				 //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

	// create a FONT object
	D3DXCreateFont(m_pD3DDevice, 30, 0, FW_BOLD, 0, false, 
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, TEXT("Delicious-Roman"), 
		&m_pD3DFont);

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Object Inits																							 //
///////////////////////////////////////////////////////////////////////////////////////////////////////////
	Player = new Object_Player();
	Player->position = D3DXVECTOR4(0.0f, 5.0f, -8.0f, 0.0f);
	Player->shape = CAPSULE;

	Mansion = new Object_Base();
	Mansion->position = D3DXVECTOR4(61.75f, 0.0f, 8.5f, 0.0f);
	Mansion->scale = D3DXVECTOR3(0.20f, 0.20f, 0.20f);
	Mansion->shape = BOX;
	Mansion->weight = UNMOVABLE;

	for(short i = 0; i < ARRAYSIZE(piano); ++i)
	{
		piano[i] = new Object_Base();
		piano[i]->shape = BOX;
		piano[i]->weight = UNMOVABLE;
		piano[i]->scale = D3DXVECTOR3(4.0f, 4.0f, 4.0f);
	}

	piano[0]->position = D3DXVECTOR4(-56.0f, 3.0f, 47.0f, 0.0f);
	piano[0]->rotation = D3DXVECTOR3(10.0f, 0.0f, 0.0f);
	piano[1]->position = D3DXVECTOR4(-12.4f, -10.8f, 0.0f, 0.0f);

	
	sinkCounter = new Object_Base();
	sinkCounter->shape = BOX;
	sinkCounter->weight = UNMOVABLE;
	sinkCounter->position = D3DXVECTOR4(-14.0f, 7.0f, 15.5f, 0.0f);
	sinkCounter->rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//for(short i = 0; i < ARRAYSIZE(normalCounter); ++i)
	//{
	//	normalCounter[i] = new Object_Base();
	//	normalCounter[i]->shape = BOX;
	//	normalCounter[i]->weight = UNMOVABLE;
	//}

	fridge = new Object_Base();
	fridge->shape = BOX;
	fridge->weight = UNMOVABLE;
	fridge->scale = D3DXVECTOR3(1.0f, 3.0f, 1.0f);
	fridge->position = D3DXVECTOR4(-7.0f, 7.0f, 15.5f, 0.0f);

	//for(short i = 0; i < ARRAYSIZE(islandCounter); ++i)
	//{
	//	islandCounter[i] = new Object_Base();
	//	islandCounter[i]->shape = BOX;
	//	islandCounter[i]->weight = UNMOVABLE;
	//}

	for(short i = 0; i < ARRAYSIZE(table); ++i)
	{
		table[i] = new Object_Base();
		table[i]->shape = BOX;
		table[i]->weight = UNMOVABLE;
		table[i]->scale = D3DXVECTOR3(2.0f, 1.25f, 2.0f);
	}
	
	// Table Position
	table[0]->position = D3DXVECTOR4(-48.8f, 2.5f, 15.5, 0.0f);
	table[1]->position = D3DXVECTOR4( -2.4f, 2.5f, 46.2, 0.0f);
	table[2]->position = D3DXVECTOR4( 44.0f, 2.5f, 20.0, 0.0f);
	table[3]->position = D3DXVECTOR4( 44.0f, 2.5f, 24.0, 0.0f);
	table[4]->position = D3DXVECTOR4( 44.0f, 2.5f, 28.0, 0.0f);
	table[5]->position = D3DXVECTOR4( 44.0f, 2.5f, 32.0, 0.0f);
	table[6]->position = D3DXVECTOR4( 44.0f, 2.5f, 36.0, 0.0f);
	table[7]->position = D3DXVECTOR4( 44.0f, 2.5f, 40.0, 0.0f);
	table[8]->position = D3DXVECTOR4( 44.0f, 2.5f, 16.0, 0.0f);
	table[9]->position = D3DXVECTOR4( 44.0f, 2.5f, 12.0, 0.0f);
	table[10]->position = D3DXVECTOR4( 44.0f, 2.5f, 8.0, 0.0f);
	table[11]->position = D3DXVECTOR4( 44.0f, 2.5f, 4.0, 0.0f);

	for(short i = 0; i < ARRAYSIZE(candleStick); ++i)
	{
		candleStick[i] = new Object_Base();
		candleStick[i]->shape = BOX;
		candleStick[i]->weight = LIGHT;
		candleStick[i]->scale = D3DXVECTOR3(0.50f, 0.50f, 0.50f);
	}
	candleStick[0]->position = D3DXVECTOR4(-48.8f, 12.8f, 15.5f, 0.0f);
	candleStick[1]->position = D3DXVECTOR4( -2.4f, 12.8f, 46.2f, 0.0f);
	candleStick[2]->position = D3DXVECTOR4( 44.0f, 12.8f, 32.0f, 0.0f);
	candleStick[3]->position = D3DXVECTOR4( 44.0f, 12.8f, 12.0f, 0.0f);
															 
	for(short i = 0; i < ARRAYSIZE(chair); ++i)
	{
		chair[i] = new Object_Base();
		chair[i]->shape = BOX;
		chair[i]->weight = HEAVY;
		chair[i]->scale = D3DXVECTOR3(1.5f, 1.5f, 1.5f);
	}

	// Chair Positions

	// Ballroom
	chair[0]->position = D3DXVECTOR4(-54.8f, 10.4f, 15.5f, 0.0f);

	chair[8 ]->position = D3DXVECTOR4(-60.0f, 10.4f,  8.2f, 0.0f);
	chair[9 ]->position = D3DXVECTOR4(-60.0f, 10.4f,  4.4f, 0.0f);
	chair[10]->position = D3DXVECTOR4(-60.0f, 10.4f,  0.5f, 0.0f);

	chair[8 ]->rotation = D3DXVECTOR3(180.0f, 0.0f, 0.0f);
	chair[9 ]->rotation = D3DXVECTOR3(180.0f, 0.0f, 0.0f);
	chair[10]->rotation = D3DXVECTOR3(180.0f, 0.0f, 0.0f);

	chair[11]->position = D3DXVECTOR4(-30.0f, 10.4f, 16.2f, 0.0f);
	chair[12]->position = D3DXVECTOR4(-30.0f, 10.4f, 12.4f, 0.0f);
	chair[13]->position = D3DXVECTOR4(-30.0f, 10.4f,  8.5f, 0.0f);
	chair[14]->position = D3DXVECTOR4(-30.0f, 10.4f,  4.2f, 0.0f);
	chair[15]->position = D3DXVECTOR4(-30.0f, 10.4f,  0.2f, 0.0f);

	chair[11]->rotation = D3DXVECTOR3( 90.0f, 0.0f, 0.0f);
	chair[12]->rotation = D3DXVECTOR3( 90.0f, 0.0f, 0.0f);
	chair[13]->rotation = D3DXVECTOR3( 90.0f, 0.0f, 0.0f);
	chair[14]->rotation = D3DXVECTOR3( 90.0f, 0.0f, 0.0f);
	chair[15]->rotation = D3DXVECTOR3( 90.0f, 0.0f, 0.0f);

	chair[16]->position = D3DXVECTOR4(-54.0f, 10.4f, -30.0f, 0.0f);
	chair[17]->position = D3DXVECTOR4(-48.0f, 10.4f, -30.0f, 0.0f);
	chair[18]->position = D3DXVECTOR4(-32.0f, 10.4f, -30.0f, 0.0f);
	chair[19]->position = D3DXVECTOR4(-35.0f, 10.4f, -30.0f, 0.0f);

	chair[16]->rotation = D3DXVECTOR3(-90.0f, 0.0f, 0.0f);
	chair[17]->rotation = D3DXVECTOR3(-90.0f, 0.0f, 0.0f);
	chair[18]->rotation = D3DXVECTOR3(-90.0f, 0.0f, 0.0f);
	chair[19]->rotation = D3DXVECTOR3(-90.0f, 0.0f, 0.0f);


											 
	// Dining Room							 
	chair[2]->position = D3DXVECTOR4( 49.0f, 10.4f, 24.4, 0.0f);
	chair[3]->position = D3DXVECTOR4( 49.0f, 10.4f, -0.5, 0.0f);
	chair[4]->position = D3DXVECTOR4( 49.0f, 10.4f, 15.5, 0.0f);
									   
	chair[2]->rotation = D3DXVECTOR3( 90.0f, 0.0f, 0.0f);
	chair[3]->rotation = D3DXVECTOR3( 90.0f, 0.0f, 0.0f);
	chair[4]->rotation = D3DXVECTOR3( 90.0f, 0.0f, 0.0f);

	chair[5]->position = D3DXVECTOR4( 39.0f, 10.4f, 46.2, 0.0f);
	chair[6]->position = D3DXVECTOR4( 39.0f, 10.4f, 24.4, 0.0f);
	chair[7]->position = D3DXVECTOR4( 39.0f, 10.4f, -0.5, 0.0f);

	chair[5]->rotation = D3DXVECTOR3(-90.0f, 0.0f, 0.0f);
	chair[6]->rotation = D3DXVECTOR3(-90.0f, 0.0f, 0.0f);
	chair[7]->rotation = D3DXVECTOR3(-90.0f, 0.0f, 0.0f);


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Load Shader Effects																					 //
///////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	// Toon Effects Shader
	ID3DXBuffer* ErrorMessage = 0;
	D3DXCreateEffectFromFile(m_pD3DDevice,
							//"ToonColored.fx",
							L"TestShader.txt",
							0,
							0,
							D3DXSHADER_DEBUG,
							0,
							&fx[0],
							&ErrorMessage);
	if( ErrorMessage )
	{
		char error[512];
		ZeroMemory( error, 256 );
		strcpy_s( error, (char*)ErrorMessage->GetBufferPointer() );
		MessageBox(0, (LPCWSTR)error, L"Shader Error", MB_OK );
	}

	//hTech[0] = fx[0]->GetTechniqueByName("ToonColored");
	hTech[0] = fx[0]->GetTechniqueByName("tech0");


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Create 3D Mesh From X																				 //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Load Test Mesh
	loadMesh(L"FlippedY.X", &Player->objectMesh);
	loadMesh(L"completedHouseFile.X", &Mansion->objectMesh); 

	for(short i = 0; i < ARRAYSIZE(piano); ++i)
	loadMesh(L"Piano.X", &piano[i]->objectMesh);

	//loadMesh(L"Sink.X", &sinkCounter->objectMesh);

	//for(short i = 0; i < ARRAYSIZE(normalCounter); ++i)
	//loadMesh(L"counter.X", &normalCounter[i]->objectMesh);

	loadMesh(L"Fridge.X", &fridge->objectMesh);

	//for(short i = 0; i < ARRAYSIZE(islandCounter); ++i)
	//loadMesh(L"counterIsland.X", &islandCounter[i]->objectMesh);

	for(short i = 0; i < ARRAYSIZE(table); ++i)
	loadMesh(L"Table.X", &table[i]->objectMesh);

	for(short i = 0; i < ARRAYSIZE(candleStick); ++i)
	loadMesh(L"CandleStick.X", &candleStick[i]->objectMesh);

	for(short i = 0; i < ARRAYSIZE(chair); ++i)
	loadMesh(L"Chair.X", &chair[i]->objectMesh);
	

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Create 3D Mesh From X																				 //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Input Manager Init
	m_pDInput = new InputManager();
	m_pDInput->init(hInst,hWnd);

	m_pDInput->SetWindowDimension(screenWidth, screenHeight);
///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Create Havok Object																					 //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Havok
	havok = new HavokCore(true);

	// Locking is necessary to make sure no two threads are trying to change the world at the same time
	havok->getWorld()->lock();

	Player->createHavokObject(havok->getWorld());

	for(short i = 0; i < ARRAYSIZE(Player->bull); ++i)
	{
		Player->bullets.push_back(0);
		Player->createBulletHavokObject(havok->getWorld(), D3DXVECTOR3(i * 20.0f, -100.0f, 0.0f), i); 
	}


	Mansion->createHavokObject(havok->getWorld());

	// Mansion
	createGroundBox(havok->getWorld(), 85.0f, 2.0f, 50.0f, 0.0f, 0.0f, 10.0f);		// Floor
	createGroundBox(havok->getWorld(), 100.0f, 20.0f, 2.0f, 0.0f, 0.0f, -40.0f);	// Front
	createGroundBox(havok->getWorld(), 100.0f, 20.0f, 2.0f, 0.0f, 0.0f, 62.5f);		// Back
	createGroundBox(havok->getWorld(), 2.0f, 20.0f, 50.0f, 71.5f, 0.0f, 10.0f);		// Right
	createGroundBox(havok->getWorld(), 2.0f, 20.0f, 50.0f, -67.5f, 0.0f, 10.0f);	// Left
	createGroundBox(havok->getWorld(), 22.5f, 5.0f, 1.5f, 2.0f, 15.0f, 14.0f);		// Middle Top
	createGroundBox(havok->getWorld(), 8.0f, 5.0f, 1.5f, 13.0f, 5.0f, 14.0f);		// Middle Right
	createGroundBox(havok->getWorld(), 8.0f, 5.0f, 1.5f, -10.0f, 5.0f, 14.0f);		// Middle Left
	createGroundBox(havok->getWorld(), 2.0f, 5.0f, 50.0f, -20.0f, 15.0f, 10.0f);	// Left Inside Top
	createGroundBox(havok->getWorld(), 2.0f, 5.0f, 50.0f, 22.5f, 15.0f, 10.0f);		// Right Inside Top
	createGroundBox(havok->getWorld(), 2.0f, 10.0f, 24.0f, -20.0f, 5.0f, 2.5f);		// Middle Left Inside
	createGroundBox(havok->getWorld(), 2.0f, 10.0f, 24.0f, 22.5f, 5.0f, 2.5f);		// Middle Right Inside
	createGroundBox(havok->getWorld(), 2.0f, 10.0f, 16.0f, -20.0f, 5.0f, 43.0f);	// Top Left Inside 
	createGroundBox(havok->getWorld(), 2.0f, 10.0f, 16.0f, 22.5f, 5.0f, 43.0f);		// Top Right Inside
	createGroundBox(havok->getWorld(), 2.0f, 10.0f, 7.5f, -20.0f, 5.0f, -37.5f);	// Bottom Left Inside 
	createGroundBox(havok->getWorld(), 2.0f, 10.0f, 7.5f, 22.5f, 5.0f, -37.5f);		// Bottom Right Inside
	createGroundBox(havok->getWorld(), 20.0f, 1.0f, 5.0f, 0.0f, 10.0f, 10.0f);		// Foyer Second Floor

	createRamp(havok->getWorld(), 5.0f, 5.0f, 5.0f, -14.0f, 0.5f, -2.0f);			// Left Stairs
	createRamp(havok->getWorld(), 5.0f, 5.0f, 5.0f, 14.0f, 0.5f, -2.0f);			// Right Stairs


	// House Objects
	for(short i = 0; i < ARRAYSIZE(piano); ++i)
		piano[i]->createHavokObject(havok->getWorld());

	//for(short i = 0; i < ARRAYSIZE(sinkCounter); ++i)
	//	sinkCounter[i]->createHavokObject(havok->getWorld());

	//for(short i = 0; i < ARRAYSIZE(normalCounter); ++i)
	//	normalCounter[i]->createHavokObject(havok->getWorld());

		fridge->createHavokObject(havok->getWorld());

	//for(short i = 0; i < ARRAYSIZE(islandCounter); ++i)
	//	islandCounter[i]->createHavokObject(havok->getWorld());

	for(short i = 0; i < ARRAYSIZE(table); ++i)
		table[i]->createHavokObject(havok->getWorld());

	for(short i = 0; i < ARRAYSIZE(candleStick); ++i)
		candleStick[i]->createHavokObject(havok->getWorld());

	for(short i = 0; i < ARRAYSIZE(chair); ++i)
		chair[i]->createHavokObject(havok->getWorld());

	/// TODO: Level Manager Havok stuff - Heather

	eventMan = new EventManager();
	eventMan->Init();

	// initialize the levels and the enemies
	levelManager.Init( m_pD3DDevice, render, havok->getWorld(), Player);
	
	havok->getWorld()->unlock();

	/// YOU FORGOT TO INIT YOU'RE VERTEX DECLARATIONS...
	/// you digital dummy :)
	InitAllVertexDeclarations();
	
	//Gamestate
	gameState = new GameStateManager();
	gameState->Init(m_hWnd,&D3Dpp,hInst,m_pD3DDevice);

	// Entity Manager
	entityMan = new EntityManager();


	for(short i = 0; i < ARRAYSIZE(candleStick); ++i)
	{
		candleStick[i]->scale = D3DXVECTOR3(0.0050f, 0.0050f, 0.0050f);
	}

	fridge->scale = D3DXVECTOR3(0.0050f, 0.0050f, 0.0050f);
	videoIsPlaying = false;
	
}

HWND CDirectXFramework::getMainWnd()
{
	return m_hWnd;
}

void CDirectXFramework::Update(float dt)
{

	if(gameState->activeGameState == GAME)
	{
		m_pDInput->Update();
		havok->stepSimulation(dt);

		havok->getWorld()->lock();
		collisions(dt);

		// Player Update
		Player->Update(dt, eyePos, lookAt, havok->getWorld());

		//minimap player position init
		gameState->setPlayerPosition(Player->position);
		//gameState->setEnemyPosition(redGhost->GetPosition());
		
		// Object Updates
		Mansion->Update(dt);

		for(short i = 0; i < ARRAYSIZE(piano); ++i)
			piano[i]->Update(dt);

		fridge->Update(dt);

		for(short i = 0; i < ARRAYSIZE(table); ++i)
			table[i]->Update(dt);

		for(short i = 0; i < ARRAYSIZE(candleStick); ++i)
			candleStick[i]->Update(dt);

		for(short i = 0; i < ARRAYSIZE(chair); ++i)
			chair[i]->Update(dt);

		if(eventMan->checkForPlayer(Player))
		{
			bool touch = true;
		}

		// update the levels and the enemies
		levelManager.Update( dt, Player, type, eyePos, lookAt);
			
		havok->getWorld()->unlock();

		UpdateCamera(dt);
		playerControls(dt);
	}
	gameState->Update(dt);
}

void CDirectXFramework::Render(float dt)
{
	if(gameState->optionsMenu != NULL)
		videoIsPlaying = gameState->optionsMenu->GetVideoPlaying();
	if(videoIsPlaying)
		return;
	// If the device was not created successfully, return
	if(!m_pD3DDevice)
		return;
	//*************************************************************************

	m_pD3DDevice->Clear(0,NULL,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,D3DCOLOR_XRGB(0,125,255),1.0f,0);
	//////////////////////////////////////////////////////////////////////////
	// All draw calls between swap chain's functions, and pre-render and post- 
	// render functions (Clear and Present, BeginScene and EndScene)
	//////////////////////////////////////////////////////////////////////////

	// Clear the back buffer, call BeginScene()
	m_pD3DDevice->BeginScene();
	//////////////////////////////////////////////////////////////////////////
	// Draw 3D Objects (for future labs - not used in Week #1)
	//////////////////////////////////////////////////////////////////////////
	m_pD3DSprite->Begin(D3DXSPRITE_ALPHABLEND);
	D3DXMatrixIdentity(&rotMat);
	D3DXMatrixIdentity(&scaleMat);
	D3DXMatrixIdentity(&worldMat);
	D3DXMatrixIdentity(&transMat);

if(gameState->activeGameState == GAME)
{
	m_pD3DDevice->SetStreamSource(0, mesh_vb, 0, sizeof(Vertex));
	m_pD3DDevice->SetIndices(mesh_ib);
	m_pD3DDevice->SetVertexDeclaration(d3dVertexDecl);

	fx[0]->SetTechnique(hTech[0]);

	UINT numPasses = 0;
	fx[0]->Begin(&numPasses, 0);

	for(UINT i = 0; i < numPasses; ++i)
	{
		fx[0]->BeginPass(i);

		D3DXMatrixRotationY(&rotMat, D3DXToRadian(Player->rotation.x));

		D3DXVECTOR3 tempPos = D3DXVECTOR3(0.0f, 0.0f, -15.0f);

		D3DXVec3Normalize(&tempPos, &tempPos);

		D3DXVec3TransformCoord(&tempPos, &tempPos, &rotMat);

		// Mesh Matrix
		D3DXMatrixScaling(&scaleMat, 0.04f, 0.04f, 0.0f);

		//D3DXMatrixRotationYawPitchRoll(&rotMat, Player->rotation.x, Player->rotation.y, Player->rotation.z);
		//D3DXMatrixRotationYawPitchRoll(&rotMat, , 0.0f, 0.0f);
		D3DXMatrixTranslation(&transMat, Player->position.x + (tempPos.x * 2) - 0.5f, Player->position.y + tempPos.y - 5.0f, Player->position.z + (tempPos.z * 2) - 0.5f); //x-1.55 is the value for gun to be directly in the center of the camera
		D3DXMatrixMultiply(&scaleMat, &scaleMat, &rotMat);
		D3DXMatrixMultiply(&worldMat, &scaleMat, &transMat);
		//D3DXMatrixMultiply(&worldMat, &scaleMat, &transMat);

		D3DXMatrixInverse(&invTransMat, 0, &worldMat);
		D3DXMatrixTranspose(&invTransMat, &invTransMat);

		D3DXMATRIX wvp = worldMat * viewMat * projMat;
		D3DXMATRIX wvpit;
		D3DXMatrixInverse(&wvpit, 0, &wvp);
		D3DXMatrixTranspose(&wvpit, &wvpit);

		fx[0]->SetMatrix("WVP", &wvp);
		fx[0]->SetMatrix("WVPIT", &wvpit);
		fx[0]->SetMatrix("World", &worldMat);
		fx[0]->SetMatrix("View", &viewMat);
		fx[0]->SetMatrix("Projection", &projMat);
		fx[0]->SetMatrix("WorldInverseTranspose", &invTransMat);

		for( short e = 0; e < Player->objectMesh->numMaterials; ++e )
		{
			fx[0]->SetTexture("gTexture", Player->objectMesh->textures[e]);
			fx[0]->CommitChanges();
			Player->objectMesh->p_Mesh->DrawSubset(e);
		}

		fx[0]->EndPass();
	}
	fx[0]->End();


	renderObject(Mansion, D3DXVECTOR3( 7.5f, -5.0, -67.5));

	// render the levels and the enemies
	levelManager.Render( m_hWnd, viewMat, projMat, eyePos);

	// Object Renders
	for(short i = 0; i < ARRAYSIZE(piano); ++i)
		renderObject(piano[i], D3DXVECTOR3(0.0f, -4.5f, 0.0f));

	//for(short i = 0; i < ARRAYSIZE(sinkCounter); ++i)
	//	renderObject(sinkCounter[i]);

	//for(short i = 0; i < ARRAYSIZE(normalCounter); ++i)
	//	renderObject(normalCounter[i]);

	renderObject(fridge, D3DXVECTOR3(-0.5f, -12.0f, 0.0f));

	//for(short i = 0; i < ARRAYSIZE(islandCounter); ++i)
	//	renderObject(islandCounter[i]);

	for(short i = 0; i < ARRAYSIZE(table); ++i)
		renderObject(table[i], D3DXVECTOR3(0.0f, -7.25f, 0.0f));

	for(short i = 0; i < ARRAYSIZE(candleStick); ++i)
		renderObject(candleStick[i], D3DXVECTOR3(-0.0f, -6.75f, 0.0f));

	for(short i = 0; i < ARRAYSIZE(chair); ++i)
		renderObject(chair[i], D3DXVECTOR3(0.0f, -8.0f, 0.5f));

	levelManager.Render( m_hWnd, viewMat, projMat, eyePos);


	Player->mPSys->draw(m_hWnd, eyePos, viewMat * projMat); // bullet draw

	//////////////////////////////////////////////////////////////////////////
	// Draw 2D sprites
	//////////////////////////////////////////////////////////////////////////
	// Note: You should only be calling the sprite object's begin and end once, 
	// with all draw calls of sprites between them

	// Call Sprite's Begin to start rendering 2D sprite objects

	//////////////////////////////////////////////////////////////////////////
	// Matrix Transformations to control sprite position, scale, and rotate
	// Set these matrices for each object you want to render to the screen
	//////////////////////////////////////////////////////////////////////////

	/*
	//drawing the sprite
	*/
	//we can specify flags that determine how subsequent sprites will be rendered. 
	//A common flag here is to say that our sprites are going to be rendered with 
	//alpha blending available D3DXSPRITE_ALPHABLEND. 
	m_pD3DSprite->Begin(D3DXSPRITE_ALPHABLEND);

	// Scaling scaleMat
	// Rotation on Z axis, value in radians, converting from degrees rotMat
	// Translation transMat
	// Multiply scale and rotation, store in scale
	// Multiply scale and translation, store in world worldMat

	// Set Transform

	D3DXMatrixIdentity(&transMat);
	D3DXMatrixIdentity(&scaleMat);
	D3DXMatrixIdentity(&rotMat);
	D3DXMatrixIdentity(&worldMat);
	D3DXMatrixScaling(&scaleMat, 1.0f, 1.0f, 0.0f);
	D3DXMatrixRotationZ(&rotMat, D3DXToRadian(0.0f));
	D3DXMatrixTranslation(&transMat, 0.0f, 0.0f, 0.0f);
	D3DXMatrixMultiply(&scaleMat, &scaleMat, &rotMat);
	D3DXMatrixMultiply(&worldMat, &scaleMat, &transMat);
	// Set Transform for the object m_pD3DSprite
	m_pD3DSprite->SetTransform(&worldMat);



	// Draw the texture with the sprite object
	//m_pD3DSprite->Draw(m_pTexture[1], 0, &D3DXVECTOR3(m_imageInfo.Width * 0.5f, 
	//	m_imageInfo.Height * 0.5f, 0.0f), 0,
	//	D3DCOLOR_ARGB(255, 255, 255, 255));
}

gameState->Render(m_pD3DSprite);
	// End drawing 2D sprites
	m_pD3DSprite->End();

	//////////////////////////////////////////////////////////////////////////
	// Draw Text
	//////////////////////////////////////////////////////////////////////////

	// Calculate RECT structure for text drawing placement, using whole screen
	RECT rect;
	GetWindowRect(m_hWnd, &rect);
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;
	//int currentRoom = eventMan->currentRoom;

	// Draw Text, using DT_TOP, DT_RIGHT for placement in the top right of the
	// screen.  DT_NOCLIP can improve speed of text rendering, but allows text
	// to be drawn outside of the rect specified to draw text in.


	// EndScene, and Present the back buffer to the display buffer
	m_pD3DDevice->EndScene();
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);


	//*************************************************************************

}

void CDirectXFramework::Shutdown()
{
	//*************************************************************************
	// Release COM objects in the opposite order they were created in

	if(gCamera)
	{
		delete gCamera;
		gCamera = 0;
	}

	// Texture
	//SAFE_RELEASE(m_pTexture)

	// Sprite
	SAFE_RELEASE(m_pD3DSprite)

	// Font
	SAFE_RELEASE(m_pD3DFont)

	// 3DDevice	
	SAFE_RELEASE(m_pD3DDevice)

	// 3DObject
	SAFE_RELEASE(m_pD3DObject)
	//*************************************************************************

}

void CDirectXFramework::loadMesh(LPCWSTR fileName, Mesh** meshObject)
{
	// Create a Temp Mesh
	Mesh* temp = new Mesh();

	// Load Mesh to Temp
	D3DXLoadMeshFromX(fileName, D3DXMESH_SYSTEMMEM, m_pD3DDevice, &adjBuffer, &matBuffer, &effBuffer, &temp->numMaterials, &temp->p_Mesh);

	// Vertex Declarations
	D3DVERTEXELEMENT9 elems[65];
	UINT numElems = 0;
	d3dVertexDecl->GetDeclaration( elems, &numElems );

	ID3DXMesh* t = 0;

	temp->p_Mesh->CloneMesh( D3DXMESH_MANAGED, elems, m_pD3DDevice, &t );
	temp->p_Mesh->Release();
	temp->p_Mesh = t;

	temp->p_Mesh->Optimize(D3DXMESH_MANAGED,
									(DWORD*)adjBuffer->GetBufferPointer(),
									0, 0, 0,
									&temp->p_Mesh);
	
	// Compute Normals (Doesn't Always Look Good)
	//D3DXComputeNormals( temp->p_Mesh, NULL );
	
	// Check for Materials already in the Mesh
	if( matBuffer )
	{
		D3DXMATERIAL* mat = (D3DXMATERIAL*)matBuffer->GetBufferPointer();
		temp->textures.clear();
		temp->textures.reserve( temp->numMaterials );

		short nummaterials = temp->GetNumMaterials();

		for( short i = 0; i < temp->GetNumMaterials(); ++i )
		{
			temp->textures.push_back( NULL );

			if( mat[i].pTextureFilename != 0 )
			{
				D3DXCreateTextureFromFileA( m_pD3DDevice, (LPCSTR)mat[i].pTextureFilename, &temp->textures[i] );
			}
		}
	}
	// Else it will just appear black (with Toon Shading)

	// Load Mesh Object with the temp variables
	*meshObject = temp;
}


void CDirectXFramework::createGroundBox(hkpWorld* world, float scaleX, float scaleY, float scaleZ, float posX, float posY, float posZ)
{
	// Create a ground area
	hkVector4 halfExtents(scaleX, scaleY, scaleZ);
	hkpBoxShape* boxShape = new hkpBoxShape(halfExtents);

	// Set its properties
	hkpRigidBodyCinfo ci;
	ci.m_shape = boxShape;
	ci.m_position = hkVector4(posX, posY, posZ);
	ci.m_motionType = hkpMotion::MOTION_FIXED;
	ci.m_friction = 1.0f;

	// Create the rigid body
	hkpRigidBody* rigidBody = new hkpRigidBody(ci);

	// No longer need the reference on the boxShape, as the rigidBody now owns it
	boxShape->removeReference();

	// Remove reference and add the rigidbody to the world
	world->addEntity(rigidBody)->removeReference();

}

void CDirectXFramework::UpdateCamera(float dt)
{
	D3DXVECTOR3 tempPos;
	D3DXMATRIX	tempRot;
	D3DXVECTOR3 tempChange;

	D3DXMatrixIdentity(&tempRot);

	D3DXMatrixRotationYawPitchRoll(&tempRot, D3DXToRadian(Player->rotation.x), D3DXToRadian(Player->rotation.y), 0.0f );

	tempPos = D3DXVECTOR3(0.0f, 0.5f, -1.0f);

	D3DXVec3TransformCoord(&tempChange, &tempPos, &tempRot);
	// Initialize View Matrix
	eyePos								= D3DXVECTOR3(0.0f, 0.5f, 0.0f ) + D3DXVECTOR3(Player->position.x, Player->position.y - 5.0f, Player->position.z);		// Camera Position
	lookAt								= D3DXVECTOR3(Player->position.x, Player->position.y - 5.0f, Player->position.z) + tempChange;							// Position camera is viewing
	upVec								= D3DXVECTOR3(0.0f, 1.0f, 0.0f);																						// Rotational orientation


	// Easily calculate the view matrix with 3 intuitive vectors
	D3DXMatrixLookAtLH(&viewMat,											// Returned viewMat
						&eyePos,											// Eye Position
						&lookAt,											// LookAt Position
						&upVec);											// Up Vector

	// Apply the view matrix in the scene
	//m_pD3DDevice->SetTransform(D3DTS_VIEW, &viewMat);

	// Initialize perspective projection matrix, this creates view frustum
	D3DXMatrixPerspectiveFovLH(&projMat,									// Return Projection Matrix
								D3DXToRadian(65.0f),						// Field of View
								(float)screenWidth / (float)screenHeight,	// Aspect Ratio
								1.0f,										// Near Plane
								1000.0f);


	// Apply the projection matrix in the scene
	//m_pD3DDevice->SetTransform(D3DTS_PROJECTION, &projMat);

	m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
}

void CDirectXFramework::playerControls(float dt)
{
	m_pDInput->getInput();

	//OMFG@W0RKINGP3WP3W
	static float delay = 0.0f;
	/*if( gDInput->keyDown(DIK_SPACE) && delay <= 0.0f)|
	{												   |
		delay = 0.3f;								   |
		Player->mPSys->addParticle();				   | this is the same as the one below, only using spacebar to shoot
	}												   |
	delay -= dt;									   |*/	

	if( m_pDInput->isButtonDown(0) && delay <= 0.0f)
	{
		delay = 0.3f;
		AudioManager::GetInstance()->PlaySFX(*gunSFX);
		Player->mPSys->addParticle(eyePos, eyePos, lookAt);
		Player->bull[Player->mPSys->GetBulletCounter() - 1].position = eyePos;
		D3DXVec3Normalize(&Player->bull[Player->mPSys->GetBulletCounter() - 1].velocity, &(eyePos - lookAt));
		//Player->createBulletHavokObject(havok->getWorld(), D3DXVECTOR3(20, -120, 0.0f), 0);
		gameState->setHudBulletCounter(Player->mPSys->GetBulletCounter());
	}
	delay -= dt;

	// Bullet Controls

	//switching from green to blue bullets
	if( m_pDInput->keyPress(DIK_1))
	{
		if(type !=green)
		{
			AudioManager::GetInstance()->PlaySFX(*changeBullet);
			type = green;
			Player->changeGunType(type);
		}
	}
	if( m_pDInput->keyPress(DIK_2) )
	{
		if(type != blue)
		{
			AudioManager::GetInstance()->PlaySFX(*changeBullet);
			type = blue;
			Player->changeGunType(type);
		}
	}
	if( m_pDInput->keyPress(DIK_3) )
	{
		if(type !=red)
		{
			AudioManager::GetInstance()->PlaySFX(*changeBullet);
			type = red;
			Player->changeGunType(type);
		}
	}

	//reload reset bullets 
	if( m_pDInput->keyPress(DIK_R))
	{
		Player->mPSys->setBulletCounter(0);
		gameState->setHudBulletCounter(Player->mPSys->GetBulletCounter());

		for(int i = 0; i < ARRAYSIZE(Player->bull); i++)
		{
			Player->bull[i].Reset();
		}
	}

	/************end of cycling stuff*/
	// Moving Forward and Backward
	if(m_pDInput->keyDown(DIK_W))
	{	
		D3DXVECTOR3 direction; // where player is going to move
		D3DXVec3Normalize(&direction, &(eyePos - lookAt));
		Player->velUD = -5.0f * direction.z; // speed (how fast) * direction (which way)
		Player->velLR = 5.0f * direction.x;
	}
	else if(m_pDInput->keyDown(DIK_S))
	{	
		D3DXVECTOR3 direction;
		D3DXVec3Normalize(&direction, &(eyePos - lookAt));
		Player->velUD = 5.0f * direction.z;
		Player->velLR = -5.0f * direction.x;
	}

	// Moving Right and Left
	else if(m_pDInput->keyDown(DIK_D))
	{
		D3DXVECTOR3 direction;
		D3DXVec3Cross(&direction, &(eyePos - lookAt), &upVec);
		D3DXVec3Normalize(&direction, &direction);
		Player->velLR = -5.0f * direction.x;
		Player->velUD = 5.0f * direction.z;

	}
	else if(m_pDInput->keyDown(DIK_A))
	{	
		D3DXVECTOR3 direction;
		D3DXVec3Cross(&direction, &(eyePos - lookAt), &upVec);
		D3DXVec3Normalize(&direction, &direction);
		Player->velLR = 5.0f * direction.x;
		Player->velUD = -5.0f * direction.z;
	}

	else if(!m_pDInput->keyDown(DIK_W) && !m_pDInput->keyDown(DIK_S) || 
		(!m_pDInput->keyDown(DIK_D) && !m_pDInput->keyDown(DIK_A)))
	{
		Player->velUD = 0.0f;
		Player->velLR = 0.0f;
	}
	/*else if(!m_pDInput->keyDown(DIK_D) && !m_pDInput->keyDown(DIK_A))
	{
		Player->velLR = 0.0f;
	}*/

	Player->rotation.x += m_pDInput->getMouseMovingX();

	Player->rotation.y -= m_pDInput->getMouseMovingY();

	if(Player->rotation.y > 60.0f)
		Player->rotation.y = 60.0f;

	if(Player->rotation.y < -65.0f)
		Player->rotation.y = -65.0f;

	if(Player->rotation.x >= 360.0f)
		Player->rotation.x = 0.0f;
	else if(Player->rotation.x <= 0.0f)
		Player->rotation.x = 360.0f;

}

 void CDirectXFramework::createRamp(hkpWorld* world, float scaleX, float scaleY, float scaleZ, float posX, float posY, float posZ)
 {
 
	// Create a ground area
	hkVector4 Vertex1(-scaleX, scaleY, 0.0f);
	hkVector4 Vertex2(-scaleX, 0.0f, 0.0f);
	hkVector4 Vertex3(scaleX, 0.0f, 0.0f);
	hkpTriangleShape* triangleShape1 = new hkpTriangleShape(Vertex1, Vertex2, Vertex3, 5.0f);

	// Create a ground area
	Vertex1 = hkVector4(-scaleX, scaleY, scaleZ);
	Vertex2 = hkVector4(-scaleX, 0.0f, scaleZ);
	Vertex3 = hkVector4(scaleX, 0.0f, scaleZ);
	hkpTriangleShape* triangleShape2 = new hkpTriangleShape(Vertex1, Vertex2, Vertex3, 5.0f);

	// Set its properties
	hkpRigidBodyCinfo ci;
	// Rotation
	hkRotation rotation;
	rotation.setAxisAngle(hkVector4(0.0, -1.0, 0.0), -90 * ( D3DX_PI / 180 ));

	hkTransform transform;
	transform.setIdentity();
	transform.setRotation(rotation);
	ci.setTransform(transform);

	ci.m_shape = triangleShape1;
	ci.m_position = hkVector4(posX, posY, posZ);
	ci.m_motionType = hkpMotion::MOTION_FIXED;
	ci.m_friction = 1.0f;

	// Create the rigid body
	hkpRigidBody* rigidBody1 = new hkpRigidBody(ci);

	ci.m_shape = triangleShape2;
 	ci.m_position = hkVector4(posX, posY, posZ);
 	ci.m_motionType = hkpMotion::MOTION_FIXED;
 	ci.m_friction = 1.0f;
 
 	// Create the rigid body
	hkpRigidBody* rigidBody2 = new hkpRigidBody(ci);

	// No longer need the reference on the boxShape, as the rigidBody now owns it
	triangleShape1->removeReference();
	triangleShape2->removeReference();

 	// Remove reference and add the rigidbody to the world
	world->addEntity(rigidBody1)->removeReference();
	world->addEntity(rigidBody2)->removeReference();

 }


void CDirectXFramework::renderObject(Object_Base* object, D3DXVECTOR3 offset)
{
	fx[0]->SetTechnique(hTech[0]);

	UINT numPasses = 0;
	fx[0]->Begin(&numPasses, 0);

	for(UINT i = 0; i < numPasses; ++i)
	{
		fx[0]->BeginPass(i);

		// Mesh Matrix
		D3DXMatrixScaling(&scaleMat, object->scale.x, object->scale.y, object->scale.z);
		D3DXMatrixRotationYawPitchRoll(&rotMat, object->rotation.x, object->rotation.y, object->rotation.z);
		D3DXMatrixTranslation(&transMat, object->position.x + offset.x, object->position.y + offset.y, object->position.z + offset.z);
		D3DXMatrixMultiply(&scaleMat, &scaleMat, &rotMat);
		D3DXMatrixMultiply(&worldMat, &scaleMat, &transMat);
		//D3DXMatrixMultiply(&worldMat, &scaleMat, &transMat);

		D3DXMatrixInverse(&invTransMat, 0, &worldMat);
		D3DXMatrixTranspose(&invTransMat, &invTransMat);

		D3DXMATRIX wvp = worldMat * viewMat * projMat;
		D3DXMATRIX wvpit;
		D3DXMatrixInverse(&wvpit, 0, &wvp);
		D3DXMatrixTranspose(&wvpit, &wvpit);

		fx[0]->SetMatrix("WVP", &wvp);
		fx[0]->SetMatrix("WVPIT", &wvpit);
		fx[0]->SetMatrix("World", &worldMat);
		fx[0]->SetMatrix("View", &viewMat);
		fx[0]->SetMatrix("Projection", &projMat);
		fx[0]->SetMatrix("WorldInverseTranspose", &invTransMat);

		for( short e = 0; e < object->objectMesh->numMaterials; ++e )
		{
			if(object->objectMesh->textures[e] != NULL)
			{
				fx[0]->SetTexture("gTexture", object->objectMesh->textures[e]);
				fx[0]->CommitChanges();
				object->objectMesh->p_Mesh->DrawSubset(e);
			}
			else
			{
				fx[0]->SetTexture("gTexture", m_pTexture[0]);
				fx[0]->CommitChanges();
				object->objectMesh->p_Mesh->DrawSubset(e);
			}
		}

		fx[0]->EndPass();
	}
	fx[0]->End();
}

void CDirectXFramework::collisions(float dt)
{ 
	hkReal deltaTime = dt;
	hkVector4 Force = hkVector4(5.0f, 3.0f, 5.0f);

}