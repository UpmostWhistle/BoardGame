
#include "PSystem.h"
#include "CameraObj.h"
#include "d3dUtil.h"
#include <cassert>
#include "DirectXFramework.h"

PSystem::PSystem(const LPCWSTR fxName, 
				 const std::string& techName,
		         const LPCWSTR texName, 
			     const D3DXVECTOR3& accel,
		         const AABB& box,
				 int maxNumParticles,
		         float timePerParticle)
	 : mAccel(accel), mBox(box), mTime(0.0f),
	   mMaxNumParticles(maxNumParticles), mTimePerParticle(timePerParticle)
{
	// Allocate memory for maximum number of particles.
	mParticles.resize(mMaxNumParticles);
	mAliveParticles.reserve(mMaxNumParticles);
	mDeadParticles.reserve(mMaxNumParticles);

	// They start off all dead.
	for(int i = 0; i < mMaxNumParticles; ++i)
	{
		mParticles[i].lifeTime = -1.0f;
		mParticles[i].initialTime = 0.0f;
	}

	D3DXMatrixIdentity(&mWorld);
	D3DXMatrixIdentity(&mInvWorld);

	// Create the texture.
	HR(D3DXCreateTextureFromFile(m_pD3DDevice, texName, &mTex));

	// Create the FX.
	ID3DXBuffer* errors = 0;
	D3DXCreateEffectFromFile(m_pD3DDevice, fxName,
		0, 0, D3DXSHADER_DEBUG, 0, &mFX, &errors);
	if( errors )
		MessageBoxW(0, (LPCWSTR)errors->GetBufferPointer(), 0, 0);



	mhTech    = mFX->GetTechniqueByName(techName.c_str());
	mhWVP     = mFX->GetParameterByName(0, "gWVP");
	mhEyePosL = mFX->GetParameterByName(0, "gEyePosL");
	mhTex     = mFX->GetParameterByName(0, "gTex");
	mhTime    = mFX->GetParameterByName(0, "gTime");
	mhAccel   = mFX->GetParameterByName(0, "gAccel");
	mhViewportHeight = mFX->GetParameterByName(0, "gViewportHeight");

	// We don't need to set these every frame since they do not change.
	HR(mFX->SetTechnique(mhTech));
	HR(mFX->SetValue(mhAccel, mAccel, sizeof(D3DXVECTOR3)));
	HR(mFX->SetTexture(mhTex, mTex));

	HR(m_pD3DDevice->CreateVertexBuffer(mMaxNumParticles*sizeof(Particle),
		D3DUSAGE_DYNAMIC|D3DUSAGE_WRITEONLY|D3DUSAGE_POINTS,
		0, D3DPOOL_DEFAULT, &mVB, 0));

	bulletCounter = 0;
	type = gun;
}

PSystem::~PSystem()
{
	ReleaseCOM(mFX);
	ReleaseCOM(mTex);
	ReleaseCOM(mVB);
}

float PSystem::getTime()
{
	return mTime;
}

void PSystem::setTime(float t)
{
	mTime = t;
}

const AABB& PSystem::getAABB()const
{
	return mBox;
}

void PSystem::setWorldMtx(const D3DXMATRIX& world)
{
	mWorld = world;

	// Compute the change of coordinates matrix that changes coordinates 
	// relative to world space so that they are relative to the particle
	// system's local space.
	D3DXMatrixInverse(&mInvWorld, 0, &mWorld);
}

void PSystem::setBulletCounter( int bulletCounter)
{
	this->bulletCounter = bulletCounter;
}

void PSystem::addParticle(D3DXVECTOR3 pos, D3DXVECTOR3 iPos, D3DXVECTOR3 look)
{
	//decide which particles are going to be used with each object, either a gun or fire
	if(type == gun)
	{
		if(bulletCounter < 12)
		{
			if( mDeadParticles.size() > 0)
			{
				// Reinitialize a particle.
				Particle* p = mDeadParticles.back();
				initParticle(*p, pos, iPos, look);

				// No longer dead.
				mDeadParticles.pop_back();
				mAliveParticles.push_back(p);
			}
		}

		bulletCounter++;

		if(bulletCounter > 12)
			bulletCounter = 12;
	}

	if(type == fire)
	{
		if( mDeadParticles.size() > 0)
		{
			// Reinitialize a particle.
			Particle* p = mDeadParticles.back();
			initParticle(*p, pos, iPos, look);

			// No longer dead.
			mDeadParticles.pop_back();
			mAliveParticles.push_back(p);
		}
	}
}


void PSystem::onLostDevice()
{
	HR(mFX->OnLostDevice());

	// Default pool resources need to be freed before reset.
	ReleaseCOM(mVB);
}

void PSystem::onResetDevice()
{
	HR(mFX->OnResetDevice());

	// Default pool resources need to be recreated after reset.
	if(mVB == 0)
	{
		HR(m_pD3DDevice->CreateVertexBuffer(mMaxNumParticles*sizeof(Particle),
			D3DUSAGE_DYNAMIC|D3DUSAGE_WRITEONLY|D3DUSAGE_POINTS,
			0, D3DPOOL_DEFAULT, &mVB, 0));
	}
}


void PSystem::update(float dt, D3DXVECTOR3 eyePos, D3DXVECTOR3 look)
{
	mTime += dt;

	// Rebuild the dead and alive list.  Note that resize(0) does
	// not deallocate memory (i.e., the capacity of the vector does
	// not change).
	mDeadParticles.resize(0);
	mAliveParticles.resize(0);

	// For each particle.
	for(int i = 0; i < mMaxNumParticles; ++i)
	{
		// Is the particle dead?
  		if( (mTime - mParticles[i].initialTime) > mParticles[i].lifeTime)
		{
			mDeadParticles.push_back(&mParticles[i]);
		}
		else
		{
			mAliveParticles.push_back(&mParticles[i]);
		}
	}


	// A negative or zero mTimePerParticle value denotes
	// not to emit any particles.
	if( mTimePerParticle > 0.0f )
	{
		// Emit particles.
		static float timeAccum = 0.0f;
		timeAccum += dt;
		while( timeAccum >= mTimePerParticle )
		{
			addParticle(eyePos, eyePos, look);
			timeAccum -= mTimePerParticle;
		}
	}
}

void PSystem::draw(HWND hWnd, D3DXVECTOR3 eyePos, D3DXMATRIX viewProj)
{
	// Get camera position relative to world space system and make it 
	// relative to the particle system's local system.
	D3DXVECTOR3 eyePosW = eyePos;
	D3DXVECTOR3 eyePosL;
	D3DXVec3TransformCoord(&eyePosL, &eyePosW, &mInvWorld);

	// Set FX parameters.
	HR(mFX->SetValue(mhEyePosL, &eyePosL, sizeof(D3DXVECTOR3)));
	HR(mFX->SetFloat(mhTime, mTime));
	HR(mFX->SetMatrix(mhWVP, &(mWorld*viewProj)));

	// Point sprite sizes are given in pixels.  So if the viewport size 
	// is changed, then more or less pixels become available, which alters
	// the perceived size of the particles.  For example, if the viewport
	// is 32x32, then a 32x32 sprite covers the entire viewport!  But if
	// the viewport is 1024x1024, then a 32x32 sprite only covers a small
	// portion of the viewport.  Thus, we scale the particle's
	// size by the viewport height to keep them in proportion to the 
	// viewport dimensions.
	HWND hwnd = hWnd; // // *handle* naming it gd3dApp for the sake of not having to change more stuff
	RECT clientRect;
	GetClientRect(hwnd, &clientRect);
	HR(mFX->SetInt(mhViewportHeight, clientRect.bottom));

	UINT numPasses = 0;
	HR(mFX->Begin(&numPasses, 0));
	HR(mFX->BeginPass(0));

	HR(m_pD3DDevice->SetStreamSource(0, mVB, 0, sizeof(Particle)));
	HR(m_pD3DDevice->SetVertexDeclaration(Particle::Decl));

	AABB boxWorld;
	mBox.xform(mWorld, boxWorld);
	if( gCamera->isVisible( boxWorld ) )
	{
		// Initial lock of VB for writing.
		Particle* p = 0;
		HR(mVB->Lock(0, 0, (void**)&p, D3DLOCK_DISCARD));
		int vbIndex = 0;

		// For each living particle.
		for(UINT i = 0; i < mAliveParticles.size(); ++i)
		{
			// Copy particle to VB
			p[vbIndex] = *mAliveParticles[i];
			++vbIndex;
		}
		HR(mVB->Unlock());

		// Render however many particles we copied over.
		if(vbIndex > 0)
		{
			HR(m_pD3DDevice->DrawPrimitive(D3DPT_POINTLIST, 0, vbIndex));
		}
	}

	HR(mFX->EndPass());
	HR(mFX->End());
}

std::vector<Particle*> PSystem::getmAliveParticles()
{
	return mAliveParticles;
}
void PSystem::setPosition(D3DXVECTOR4 position)
{
	this->position = position;
}