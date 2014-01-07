#include "SoundLoader.h"

SoundLoader* SoundLoader::instance = 0;

SoundLoader* SoundLoader::GetInstance(void)
{
	if(instance == 0)
		instance = new SoundLoader();
	return instance;
}

void SoundLoader::DestroyInstance(void)
{
	if(instance != 0)
	{
		delete instance;
		instance = 0;
	}
}

SoundLoader::SoundLoader(void)
{
	for each(SoundEffect* sound in sounds)
	{
		sound = NULL;
	}
	bgMusic = NULL;
}

SoundLoader::~SoundLoader(void)
{
	FlushAll();
}

SoundEffect* SoundLoader::Load(bool isStream, bool willLoop, char* fileName)
{
	SoundEffect* newSound = new SoundEffect();
	newSound->Load(isStream, willLoop, fileName);
	if(newSound->GetSound() != NULL)
		sounds.push_back(newSound);
	return newSound;
}

SoundEffect* SoundLoader::LoadBGM(char* fileName)
{
	SAFE_DELETE(bgMusic);
	bgMusic = new SoundEffect();
	bgMusic->Load(true, true, fileName);
	return bgMusic;
}

void SoundLoader::FlushSFX(void)
{
	for each(SoundEffect* sound in sounds)
	{
		SAFE_DELETE(sound);
	}
	sounds.clear();
}

void SoundLoader::FlushAll(void)
{
	FlushSFX();
	SAFE_DELETE(bgMusic);
}
