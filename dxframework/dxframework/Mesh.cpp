#include "Mesh.h"


Mesh::Mesh(void)
{
}


Mesh::~Mesh(void)
{
}

void Mesh::ComputeWorld(void)
{
	// Create Temp Meshes for Translating
	D3DXMATRIX	transMat, rotMat, scaleMat;

	// Mesh Matrix
	D3DXMatrixScaling(&scaleMat, 0.025f, 0.025f, 0.025f);
	D3DXMatrixRotationYawPitchRoll(&rotMat, 0.0f, 0.0f, 0.0f);
	D3DXMatrixTranslation(&transMat, 0.0f, 0.0f, 0.0f);
	D3DXMatrixMultiply(&scaleMat, &scaleMat, &rotMat);
	D3DXMatrixMultiply(&world, &scaleMat, &transMat);
}

DWORD Mesh::GetNumMaterials(void)
{
	return numMaterials;
}