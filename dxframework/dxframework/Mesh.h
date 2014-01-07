#pragma once

#include <d3dx9.h>
#include <d3d9.h>
#include <vector>

#pragma comment (lib, "D3dxof.lib")

class Mesh
{

public:
	ID3DXMesh*	p_Mesh;
	std::vector<IDirect3DTexture9*> textures;
	std::vector<D3DXMATERIAL> materials;
	DWORD numMaterials;
	D3DXVECTOR3 position, rotation, scale;
	D3DXMATRIX world;

	Mesh(void);
	~Mesh(void);

	
	void ComputeWorld();
	DWORD GetNumMaterials();

	void Shutdown();
};

