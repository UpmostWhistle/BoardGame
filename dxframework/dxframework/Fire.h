#include <crtdbg.h>
#include "DirectXFramework.h"
#include "InputManager.h"
#include <list>
#include <ctime>
#include <vector>
#include "CameraObj.h"


class FireRing : public PSystem
{
public:
	FireRing(const LPCWSTR fxName, 
		const std::string& techName, 
		const LPCWSTR texName, 
		const D3DXVECTOR3& accel, 
		const AABB& box,
		int maxNumParticles,
		float timePerParticle)
		: PSystem(fxName, techName, texName, accel, box, 
		maxNumParticles, timePerParticle)
	{}

	void initParticle(Particle& out, D3DXVECTOR3 pos, D3DXVECTOR3 iPos, D3DXVECTOR3 look)
	{
		out.initialPos = D3DXVECTOR3(getPosition().x, getPosition().y-2, getPosition().z);
 
		out.initialTime     = mTime;
		out.lifeTime        = GetRandomFloat(1.0f, 5.0f);
		out.initialColor    = WHITE;
		out.initialSize     = GetRandomFloat(1.0f, 12.0f);
		out.mass            = GetRandomFloat(0.8f, 1.2f);

		out.initialVelocity.x = GetRandomFloat(-0.5f, 0.5f);
		out.initialVelocity.y = GetRandomFloat(1.0f, 2.0f);
		out.initialVelocity.z = GetRandomFloat(-0.5f, 0.5f);

	}

};
