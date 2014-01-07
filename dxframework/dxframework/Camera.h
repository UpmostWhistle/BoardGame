///////////////////////////////////////////////////////////////////////////////
//	CAMERA CLASS
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "d3d9.h"
#include "d3dx9.h"
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")

const short CURSORX = 250;
const short CURSORY = 250;

const float MAXSTRICTNESS = 20.0f;
const float MINSTRICTNESS = 1.0f;

struct AABB
{
	AABB()
	{
		Min = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
		Max = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	}
	AABB( D3DXVECTOR3 minimum, D3DXVECTOR3 maximum ) :
	Min( minimum ), Max( maximum ){}

	D3DXVECTOR3 Min;
	D3DXVECTOR3 Max;
};

class Camera
{
public:
	Camera();

	const D3DXMATRIX& view();
	const D3DXMATRIX& proj();
	const D3DXMATRIX& viewProj();

	const D3DXVECTOR3& right();
	const D3DXVECTOR3& up();
	const D3DXVECTOR3& look();

	D3DXVECTOR3& pos();

	void ResetCamera();

	void lookAt( D3DXVECTOR3& pos, D3DXVECTOR3& target, D3DXVECTOR3& up );

	void setLens( float fov, float aspect, float nearZ, float farZ );

	void setOrthoLens( float width, float height, float nearZ, float farZ );

	void setSpeed( float s );

	float follow2DXY( D3DXVECTOR3 target, float distToTarget, float followStrictness, float dt );
	float follow2DZY( D3DXVECTOR3 target, float distToTarget, float followStrictness, float dt );

	void strifeLeft( float speed, float dt );
	void strifeRight( float speed, float dt );
	void strifeUp( float speed, float dt );
	void strifeDown( float speed, float dt );
	void moveForward( float speed, float dt );
	void moveBackward( float speed, float dt );
	void rotateCameraVertical( float rot, float dt );
	void rotateCameraHorizontal( float rot, float dt );

	void RotateAroundPoint( D3DXVECTOR3 look, float speed, float dt );

	void strifeLeft( float dt );
	void strifeRight( float dt );
	void strifeUp( float dt );
	void strifeDown( float dt );
	void moveForward( float dt );
	void moveBackward( float dt );
	void rotateCameraVerticalDown( float dt );
	void rotateCameraHorizontalLeft( float dt );
	void rotateCameraVerticalUp( float dt );
	void rotateCameraHorizontalRight( float dt );

	void update();

protected:
	void buildView();

private:
	void buildFrustum();

	
public:
	bool IsSphereVisible( D3DXVECTOR3* position, float radius );

	bool IsAABBVisible( const AABB& box ) const;

public:
	D3DXVECTOR3 GetForward();
	D3DXVECTOR3 GetRight();
	D3DXVECTOR3 GetUp();

private:

	D3DXMATRIX mView;
	D3DXMATRIX mProj;
	D3DXMATRIX mViewProj;

	D3DXVECTOR3 mPosW;
	D3DXVECTOR3 mRightW;
	D3DXVECTOR3 mUpW;
	D3DXVECTOR3 mLookW;


	D3DXPLANE FrustumPlanes[6];

	float mSpeed;
	float mRotSpeed;
};