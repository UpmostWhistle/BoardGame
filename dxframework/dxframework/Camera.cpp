#include "Camera.h"

///////////////////////////////////////////////////////////////////////////////
//	CAMERA CLASS
///////////////////////////////////////////////////////////////////////////////
Camera::Camera()
{
	D3DXMatrixIdentity( &mView );
	D3DXMatrixIdentity( &mProj );
	D3DXMatrixIdentity( &mViewProj );

	mPosW	= D3DXVECTOR3( 0.0f, 0.0f, -5.0f );
	mRightW	= D3DXVECTOR3( 1.0f, 0.0f, 0.0f );
	mUpW	= D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
	mLookW	= D3DXVECTOR3( 0.0f, 0.0f, 1.0f );

	mSpeed = 5.0f;
	mRotSpeed = 3.0f;

	SetCursorPos( CURSORX, CURSORY );
}

const D3DXMATRIX& Camera::view()
{
	return mView;
}
const D3DXMATRIX& Camera::proj()
{
	return mProj;
}
const D3DXMATRIX& Camera::viewProj()
{
	return mViewProj;
}

const D3DXVECTOR3& Camera::right()
{
	return mRightW;
}
const D3DXVECTOR3& Camera::up()
{
	return mUpW;
}
const D3DXVECTOR3& Camera::look() 
{
	return mLookW;
}

D3DXVECTOR3& Camera::pos()
{
	return mPosW;
}

void Camera::ResetCamera()
{
	mPosW	= D3DXVECTOR3( 0.0f, 0.0f, -5.0f );
	mRightW	= D3DXVECTOR3( 1.0f, 0.0f, 0.0f );
	mUpW	= D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
	mLookW	= D3DXVECTOR3( 0.0f, 0.0f, 1.0f );
	SetCursorPos( CURSORX, CURSORY );
}

void Camera::lookAt( D3DXVECTOR3& pos,
			D3DXVECTOR3& target, D3DXVECTOR3& up )
{
	D3DXVECTOR3 L = target - pos;
	D3DXVec3Normalize( &L, &L );

	D3DXVECTOR3 R;
	D3DXVec3Cross( &R, &up, &L );
	D3DXVec3Normalize( &R, &R );

	D3DXVECTOR3 U;
	D3DXVec3Cross( &U, &L, &R );
	D3DXVec3Normalize( &U, &U );

	mPosW	= pos;
	mRightW	= R;
	mUpW	= U;
	mLookW	= L;

	buildView();

	mViewProj = mView * mProj;
}

void Camera::setLens( float fov, float aspect, float nearZ, float farZ )
{
	D3DXMatrixPerspectiveFovLH( &mProj, fov, aspect, nearZ, farZ );
	mViewProj = mView * mProj;
}

void Camera::setOrthoLens( float width, float height, float nearZ, float farZ )
{
	D3DXMatrixOrthoLH(&mProj, width, height, nearZ, farZ);
	mViewProj = mView * mProj;
}

void Camera::setSpeed( float s )
{
	mSpeed = s;
}

float Camera::follow2DXY( D3DXVECTOR3 target, float distToTarget, float followStrictness, float dt )
{
	if( followStrictness < MINSTRICTNESS )
		followStrictness = MINSTRICTNESS;
	if( followStrictness > MAXSTRICTNESS )
		followStrictness = MAXSTRICTNESS;

	strifeRight( followStrictness * (target.x - mPosW.x), dt );
	strifeUp( followStrictness * (target.y - mPosW.y), dt );

	float dist = followStrictness * ( distToTarget - abs(target.z - mPosW.z) );
	moveBackward(dist, dt);

	lookAt(mPosW, target, mUpW);

	update();

	return followStrictness;
}
float Camera::follow2DZY( D3DXVECTOR3 target, float distToTarget, float followStrictness, float dt )
{
	if( followStrictness < MINSTRICTNESS )
		followStrictness = MINSTRICTNESS;
	if( followStrictness > MAXSTRICTNESS )
		followStrictness = MAXSTRICTNESS;

	strifeRight( followStrictness * (mPosW.z - target.z), dt );
	strifeUp( followStrictness * (target.y - mPosW.y), dt );

	float dist = followStrictness * ( distToTarget - abs( target.x - mPosW.x ) );
	moveBackward(dist, dt);

	lookAt( mPosW, target, mUpW );

	update();

	return followStrictness;
}

void Camera::strifeLeft( float speed, float dt )
{
	//find the net direction the camera is travelling
	D3DXVECTOR3 dir( 0.0f, 0.0f, 0.0f );

	dir -= mRightW;

	D3DXVec3Normalize( &dir, &dir );
	mPosW += dir * (speed * dt);

	update();
}
void Camera::strifeRight( float speed, float dt )
{
	//find the net direction the camera is travelling
	D3DXVECTOR3 dir( 0.0f, 0.0f, 0.0f );

	dir += mRightW;

	D3DXVec3Normalize( &dir, &dir );
	mPosW += dir * (speed * dt);

	update();
}
void Camera::strifeUp( float speed, float dt )
{
	//find the net direction the camera is travelling
	D3DXVECTOR3 dir( 0.0f, 0.0f, 0.0f );

	dir += mUpW;

	D3DXVec3Normalize( &dir, &dir );
	mPosW += dir * (speed * dt);

	update();
}
void Camera::strifeDown( float speed, float dt )
{
	//find the net direction the camera is travelling
	D3DXVECTOR3 dir( 0.0f, 0.0f, 0.0f );

	dir -= mUpW;

	D3DXVec3Normalize( &dir, &dir );
	mPosW += dir * (speed * dt);

	update();
}
void Camera::moveForward( float speed, float dt )
{
	//find the net direction the camera is travelling
	D3DXVECTOR3 dir( 0.0f, 0.0f, 0.0f );

	dir += mLookW;

	D3DXVec3Normalize( &dir, &dir );
	mPosW += dir * (speed * dt);

	update();
}
void Camera::moveBackward( float speed, float dt )
{
	//find the net direction the camera is travelling
	D3DXVECTOR3 dir( 0.0f, 0.0f, 0.0f );

	dir -= mLookW;

	D3DXVec3Normalize( &dir, &dir );
	mPosW += dir * (speed * dt);

	update();
}
void Camera::rotateCameraVertical( float rot, float dt )
{
	float pitch = rot * dt;

	D3DXMATRIX R;
	D3DXMatrixRotationAxis( &R, &mRightW, pitch );
	D3DXVec3TransformCoord( &mLookW, &mLookW, &R );
	D3DXVec3TransformCoord( &mUpW, &mUpW, &R );

	update();
}
void Camera::rotateCameraHorizontal( float rot, float dt )
{
	D3DXMATRIX R;
	D3DXMatrixRotationY( &R, rot * dt );
	D3DXVec3TransformCoord( &mRightW, &mRightW, &R );
	D3DXVec3TransformCoord( &mUpW, &mUpW, &R );
	D3DXVec3TransformCoord( &mLookW, &mLookW, &R );

	update();
}

void Camera::RotateAroundPoint( D3DXVECTOR3 look, float speed, float dt )
{
	D3DXVECTOR3 dir( 0.0f, 0.0f, 0.0f );
	D3DXVECTOR3 t;
	D3DXVec3Normalize(&t, &mRightW);
	dir += t;

	D3DXVec3Normalize( &dir, &dir );
	mPosW += dir * (speed * dt);

	lookAt( pos(), look, mUpW );

	update();
}

// using a constant value
void Camera::strifeLeft( float dt )
{
	//find the net direction the camera is travelling
	D3DXVECTOR3 dir( 0.0f, 0.0f, 0.0f );

	dir -= mRightW;

	D3DXVec3Normalize( &dir, &dir );
	mPosW += dir * (mSpeed * dt);

	update();
}
void Camera::strifeRight( float dt )
{
	//find the net direction the camera is travelling
	D3DXVECTOR3 dir( 0.0f, 0.0f, 0.0f );

	dir += mRightW;

	D3DXVec3Normalize( &dir, &dir );
	mPosW += dir * (mSpeed * dt);

	update();
}
void Camera::strifeUp( float dt )
{
	//find the net direction the camera is travelling
	D3DXVECTOR3 dir( 0.0f, 0.0f, 0.0f );

	dir += mUpW;

	D3DXVec3Normalize( &dir, &dir );
	mPosW += dir * (mSpeed * dt);

	update();
}
void Camera::strifeDown( float dt )
{
	//find the net direction the camera is travelling
	D3DXVECTOR3 dir( 0.0f, 0.0f, 0.0f );

	dir -= mUpW;

	D3DXVec3Normalize( &dir, &dir );
	mPosW += dir * (mSpeed * dt);

	update();
}
void Camera::moveForward( float dt )
{
	//find the net direction the camera is travelling
	D3DXVECTOR3 dir( 0.0f, 0.0f, 0.0f );

	dir += mLookW;

	D3DXVec3Normalize( &dir, &dir );
	mPosW += dir * (mSpeed * dt);

	update();
}
void Camera::moveBackward( float dt )
{
	//find the net direction the camera is travelling
	D3DXVECTOR3 dir( 0.0f, 0.0f, 0.0f );

	dir -= mLookW;

	D3DXVec3Normalize( &dir, &dir );
	mPosW += dir * (mSpeed * dt);

	update();
}
void Camera::rotateCameraVerticalDown( float dt )
{
	float pitch = -mRotSpeed * dt;

	D3DXMATRIX R;
	D3DXMatrixRotationAxis( &R, &mRightW, pitch );
	D3DXVec3TransformCoord( &mLookW, &mLookW, &R );
	D3DXVec3TransformCoord( &mUpW, &mUpW, &R );

	update();
}
void Camera::rotateCameraHorizontalLeft( float dt )
{
	D3DXMATRIX R;
	D3DXMatrixRotationY( &R, -mRotSpeed * dt );
	D3DXVec3TransformCoord( &mRightW, &mRightW, &R );
	D3DXVec3TransformCoord( &mUpW, &mUpW, &R );
	D3DXVec3TransformCoord( &mLookW, &mLookW, &R );

	update();
}
void Camera::rotateCameraVerticalUp( float dt )
{
	float pitch = mRotSpeed * dt;

	D3DXMATRIX R;
	D3DXMatrixRotationAxis( &R, &mRightW, pitch );
	D3DXVec3TransformCoord( &mLookW, &mLookW, &R );
	D3DXVec3TransformCoord( &mUpW, &mUpW, &R );

	update();
}
void Camera::rotateCameraHorizontalRight( float dt )
{
	D3DXMATRIX R;
	D3DXMatrixRotationY( &R, mRotSpeed * dt );
	D3DXVec3TransformCoord( &mRightW, &mRightW, &R );
	D3DXVec3TransformCoord( &mUpW, &mUpW, &R );
	D3DXVec3TransformCoord( &mLookW, &mLookW, &R );

	Camera::update();
}



void Camera::update()
{
	buildView();
	buildFrustum();

	mViewProj = mView * mProj;
}

void Camera::buildView()
{
	D3DXVec3Normalize( &mLookW, &mLookW );

	D3DXVec3Cross( &mUpW, &mLookW, &mRightW );
	D3DXVec3Normalize( &mUpW, &mUpW );

	D3DXVec3Cross( &mRightW, &mUpW, &mLookW );
	D3DXVec3Normalize( &mRightW, &mRightW );

	float x = -D3DXVec3Dot( &mPosW, &mRightW );
	float y = -D3DXVec3Dot( &mPosW, &mUpW );
	float z = -D3DXVec3Dot( &mPosW, &mLookW );

	mView(0,0) = mRightW.x;
	mView(1,0) = mRightW.y;
	mView(2,0) = mRightW.z;
	mView(3,0) = x;

	mView(0,1) = mUpW.x;
	mView(1,1) = mUpW.y;
	mView(2,1) = mUpW.z;
	mView(3,1) = y;

	mView(0,2) = mLookW.x;
	mView(1,2) = mLookW.y;
	mView(2,2) = mLookW.z;
	mView(3,2) = z;

	mView(0,3) = 0.0f;
	mView(1,3) = 0.0f;
	mView(2,3) = 0.0f;
	mView(3,3) = 1.0f;
}


void Camera::buildFrustum()
{
	D3DXMATRIX vp = mView * mProj;

	D3DXVECTOR4 col0( vp(0,0), vp(1,0), vp(2,0), vp(3,0) );
	D3DXVECTOR4 col1( vp(0,1), vp(1,1), vp(2,1), vp(3,1) );
	D3DXVECTOR4 col2( vp(0,2), vp(1,2), vp(2,2), vp(3,2) );
	D3DXVECTOR4 col3( vp(0,3), vp(1,3), vp(2,3), vp(3,3) );

	FrustumPlanes[0] = (D3DXPLANE)( col3 + col2 );			// near
	FrustumPlanes[1] = (D3DXPLANE)( col3 - col2 );	// far
	FrustumPlanes[2] = (D3DXPLANE)( col3 + col0 );	// left
	FrustumPlanes[3] = (D3DXPLANE)( col3 - col0 );	// right
	FrustumPlanes[4] = (D3DXPLANE)( col3 - col1 );	// top
	FrustumPlanes[5] = (D3DXPLANE)( col3 + col1 );	// bottom

	for( byte i = 0; i < 6; ++i )
		D3DXPlaneNormalize( &FrustumPlanes[i], &FrustumPlanes[i] );
}


bool Camera::IsSphereVisible( D3DXVECTOR3* position, float radius )
{
	//radius *= 1.2f;
	for( byte i = 0; i < 6; ++i )
	{
		if( D3DXPlaneDotCoord( &FrustumPlanes[i], position ) + radius < 0 )
		{
			// outside the frustum
			return false;
		}
	}

	return true;
}

bool Camera::IsAABBVisible( const AABB& box ) const
{
	D3DXVECTOR3 P;
	D3DXVECTOR3 Q;

	for( byte i = 0; i < 6; ++i )
	{
		for( byte j = 0; j < 3; ++j )
		{
			if( FrustumPlanes[i][j] >= 0.0f )
			{
				P[j] = box.Min[j];
				Q[j] = box.Max[j];
			}else
			{
				P[j] = box.Max[j];
				Q[j] = box.Min[j];
			}
		}

		if( D3DXPlaneDotCoord( &FrustumPlanes[i], &Q ) < 0.0f )
			return false;
	}

	return true;
}

D3DXVECTOR3 Camera::GetForward()
{
	return mLookW;
}
D3DXVECTOR3 Camera::GetRight()
{
	return mRightW;
}
D3DXVECTOR3 Camera::GetUp()
{
	return mUpW;
}