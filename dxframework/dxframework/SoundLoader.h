#pragma once

#include <list>
#include "SoundEffect.h"
#include "Utilities.h"

class SoundLoader
{
public:
	~SoundLoader(void);
	static SoundLoader* GetInstance(void);
	static void DestroyInstance(void);

	SoundEffect* Load(bool isStream, bool willLoop, char* fileName);
	SoundEffect* LoadBGM(char* fileName);
	void FlushSFX(void);
	void FlushAll(void);

private:
	SoundLoader(void);
	static SoundLoader* instance;
	std::list<SoundEffect*> sounds;
	SoundEffect* bgMusic;
};

