#pragma once

#include <d3d9.h>
#include <d3dx9.h>

#include "Object_Player.h"
#include "Object_Base.h"

// Macro to release COM objects fast and safely
#define SAFE_RELEASE(x) if(x){x->Release(); x = 0;}

class RenderObject
{
private:
	IDirect3DDevice9*			device;	// Direct3D 9 Device

	//////////////////////////////////////////////////////////////////////////
	// Structs																//
	//////////////////////////////////////////////////////////////////////////
	
	// Vertex
	struct Vertex
	{
		D3DXVECTOR3				pos;
		D3DXVECTOR3				norm;
		D3DXVECTOR2				uv;
	}tempPos;

	//////////////////////////////////////////////////////////////////////////
	// Shape Variables														//
	//////////////////////////////////////////////////////////////////////////

	UINT						Length;

	// Basic
	IDirect3DVertexBuffer9*		mesh_vb;
	IDirect3DIndexBuffer9*		mesh_ib;

	//////////////////////////////////////////////////////////////////////////
	// 3D Variables													        //
	//////////////////////////////////////////////////////////////////////////

	// scale
	D3DXVECTOR4						scale;

	// Lights
	D3DLIGHT9						light;

	// Handles
	D3DXHANDLE						hTech[10];
	D3DXHANDLE						hWVP;
	HANDLE*							pSharedHandle;

	// Vertex Declaration
	IDirect3DVertexDeclaration9*	d3dVertexDecl;

	// Double Words
	DWORD							fillMode;
	DWORD							Usage;
	DWORD							FVF;
	DWORD							mNumGridVertices;
	DWORD							mNumGridTriangles;

	// Pool
	D3DPOOL							Pool;

	// Materials
	//D3DMATERIAL9					m_pMaterial[10];
	//DWORD							numMaterials;

	// Buffers
	ID3DXBuffer*					adjBuffer;
	ID3DXBuffer*					matBuffer;
	ID3DXBuffer*					effBuffer;

	// Mesh
	ID3DXMesh*						testMesh;

	// movement variables
	float						rotation;


	//////////////////////////////////////////////////////////////////////////
	// Shaders																//
	//////////////////////////////////////////////////////////////////////////
	ID3DXEffect*					fx[10];

	//////////////////////////////////////////////////////////////////////////
	// Matricies															//
	//////////////////////////////////////////////////////////////////////////
	D3DXMATRIX						transMat;
	D3DXMATRIX						rotMat;
	D3DXMATRIX						scaleMat;
	D3DXMATRIX						worldMat;
	//D3DXMATRIX						viewMat;
	//D3DXMATRIX						projMat;
	D3DXMATRIX						invTransMat;

	//////////////////////////////////////////////////////////////////////////
	// Material Variables													//
	//////////////////////////////////////////////////////////////////////////
	D3DMATERIAL9					m_pMaterial[10];
	DWORD							numMaterials;

	//////////////////////////////////////////////////////////////////////////
	// Sprite Variables														//
	//////////////////////////////////////////////////////////////////////////
	ID3DXSprite*					spriteObject;		// Sprite Object
	IDirect3DTexture9*				m_pTexture[10];		// Texture Object for a sprite
	D3DXIMAGE_INFO					m_imageInfo;		// File details of a texture



public:
	RenderObject(void);
	~RenderObject(void);

	void Init(IDirect3DDevice9* device, ID3DXSprite* m_pD3DSprite );
	void LoadTexture( LPCWSTR fileName, int textureNum );
	void LoadMesh( LPCWSTR fileName, Mesh** meshObject);
	void LoadShaderEffects(LPCWSTR fileName, int fxNum );
	void Render3DObject(D3DXVECTOR4 position, Mesh* ObjectMesh, D3DXMATRIX	viewMat, D3DXMATRIX projMat, int textureNum);
	void Render_Object(Object_Base* object, D3DXVECTOR3 offset, D3DXMATRIX	viewMat, D3DXMATRIX projMat);
	void Render2DSprite(int textureNum);

	void MaterialSettings();

	void CreateVertexElement();

	// Accessors and Mutators
	void SetVertexDecl( IDirect3DVertexDeclaration9* d3dVertexDecl)
	{
		this->d3dVertexDecl = d3dVertexDecl;
	};

	void SetRotation( float newRotation )
	{
		rotation = newRotation;
	};

	void SetScale( D3DXVECTOR4 newScale )
	{
		scale = newScale;
	};
};

