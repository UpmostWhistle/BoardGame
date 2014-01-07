#include "CameraObj.h"

CameraObj* gCamera = 0;

CameraObj::CameraObj(void)
{
	D3DXMatrixIdentity( &viewMat );
	D3DXMatrixIdentity( &projMat );
	D3DXMatrixIdentity( &viewProjMat );

	eyePos	= D3DXVECTOR3(0.0f, -12.0f, 2.0f);			// The position of our camera
	lookAt	= D3DXVECTOR3(0.0f, 0.0f, 1.0f);			// The lookat target of our camera
	upVec	= D3DXVECTOR3(0.0f, 1.0f, 0.0f);			// Up vector
	right	= D3DXVECTOR3(1.0f, 0.0f, 0.0f);			//
}


CameraObj::~CameraObj(void)
{

}

void CameraObj::createCamera(float farPlane, float nearPlane, float aspect, float FOV)
{
	D3DXMATRIX temp;
	D3DXMatrixPerspectiveFovLH(&temp, FOV, aspect, nearPlane, farPlane);
	projMat = temp;
	//D3DXMatrixLookAtLH( &viewMat, &eyePos, &D3DXVECTOR3(0,0,0), &upVec );
	buildFrustum();
	viewProjMat = viewMat * projMat;
}

void CameraObj::updateCamera(D3DXVECTOR3 lookAtRot, D3DXVECTOR4 TargetPos)
{
	D3DXVECTOR3 tempTargetPos;

	tempTargetPos.x = TargetPos.x;
	tempTargetPos.y = TargetPos.y;
	tempTargetPos.z = TargetPos.z;

	//lookAtRot = lookAtRot;// - this->eyePos;
	//D3DXMatrixLookAtLH( &viewMat, &tempTargetPos, &lookAtRot, &upVec );

	setLook(lookAtRot, tempTargetPos, upVec);

	//D3DXVec3Normalize(&upVec, &upVec);

	//buildView();
	//buildFrustum();

	//D3DXMatrixLookAtLH( &viewMat, &tempTargetPos, &lookAtRot, &upVec );
	//D3DXMatrixMultiply(&viewProjMat, &viewMat, &projMat);
}

void CameraObj::setLook(D3DXVECTOR3 lookingAt, D3DXVECTOR3& position, D3DXVECTOR3& up)
{
	
	D3DXVECTOR3 L = lookingAt - position;
	D3DXVec3Normalize(&L, &L);

	D3DXVECTOR3 R;
	D3DXVec3Cross(&R, &up, &L);
	D3DXVec3Normalize(&R, &R);

	D3DXVECTOR3 U;
	D3DXVec3Cross(&U, &L, &R);
	D3DXVec3Normalize(&U, &U);

	eyePos = position;
	right = R;
	upVec = U;
	lookAt = L;

	buildView();
	buildFrustum();

	viewProjMat = viewMat * projMat;
}

const D3DXMATRIX& CameraObj::viewProj() const
{
	return viewProjMat;
}

D3DXVECTOR3& CameraObj::pos()
{
	return eyePos;
}

const D3DXVECTOR3& CameraObj::look() const
{
	return lookAt;
}

void CameraObj::setPosition(D3DXVECTOR3 eyePosition)
{
	eyePos = D3DXVECTOR3(eyePosition.x, eyePosition.y - 1.5f, eyePosition.z + 2.0f);
}

void CameraObj::setUp(D3DXVECTOR3 upVector)
{
	upVec = upVector;
}

void CameraObj::followPoint()
{

}

void CameraObj::buildView()
{
	D3DXVec3Normalize(&lookAt, &lookAt);

	D3DXVec3Cross(&upVec, &lookAt, &right);
	D3DXVec3Normalize(&upVec, &upVec);

	D3DXVec3Cross( &right, &upVec, &lookAt);
	D3DXVec3Normalize(&right, &right);
	
	float x = -D3DXVec3Dot(&eyePos, &right);
	float y = -D3DXVec3Dot(&eyePos, &upVec);
	float z = -D3DXVec3Dot(&eyePos, &lookAt);

	viewMat(0,0) = right.x;
	viewMat(1,0) = right.y;
	viewMat(2,0) = right.z;
	viewMat(3,0) = x;
				 
	viewMat(0,1) = upVec.x;
	viewMat(1,1) = upVec.y;
	viewMat(2,1) = upVec.z;
	viewMat(3,1) = y;
				 
	viewMat(0,2) = lookAt.x;
	viewMat(1,2) = lookAt.y;
	viewMat(2,2) = lookAt.z;
	viewMat(3,2) = z;
				 
	viewMat(0,3) = 0.0f;
	viewMat(1,3) = 0.0f;
	viewMat(2,3) = 0.0f;
	viewMat(3,3) = 1.0f;
}

void CameraObj::buildFrustum()
{
	D3DXMATRIX vp = viewMat * projMat;

	D3DXVECTOR4 col0( vp(0,0), vp(1,0), vp(2,0), vp(3,0) );
	D3DXVECTOR4 col1( vp(0,1), vp(1,1), vp(2,1), vp(3,1) );
	D3DXVECTOR4 col2( vp(0,2), vp(1,2), vp(2,2), vp(3,2) );
	D3DXVECTOR4 col3( vp(0,3), vp(1,3), vp(2,3), vp(3,3) );
														 
	Frustum[0] = (D3DXPLANE)(col2);			// Near
	Frustum[1] = (D3DXPLANE)(col3 - col2);  // Far
	Frustum[2] = (D3DXPLANE)(col3 + col0);  // Left
	Frustum[3] = (D3DXPLANE)(col3 - col0);  // Right
	Frustum[4] = (D3DXPLANE)(col3 + col1);  // Top
	Frustum[5] = (D3DXPLANE)(col3 - col1);  // Bottom

	for( byte i = 0; i < 6; ++i)
		D3DXPlaneNormalize( &Frustum[i], &Frustum[i]);
}

bool CameraObj::isVisible(const AABB& Box)const
{
	D3DXVECTOR3 P;
	D3DXVECTOR3 Q;

	for(int i = 0; i < 6; ++i)
	{
		for(int j = 0; j < 3; ++j)
		{
			if(Frustum[i][j] >= 0.0f)
			{
				P[j] = Box.minPt[j];
				Q[j] = Box.maxPt[j];
			}
			else
			{
				P[j] = Box.maxPt[j];
				Q[j] = Box.minPt[j];
			}
		}

		if(D3DXPlaneDotCoord(&Frustum[i], &Q) < 0.0f)
			return false;
	}

	return true;
}