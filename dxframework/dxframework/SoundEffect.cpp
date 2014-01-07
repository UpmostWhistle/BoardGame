#include "SoundEffect.h"

// Constructor
SoundEffect::SoundEffect(void)
{
	sound = NULL;
	fileLocation = NULL;
}

// Destructor
SoundEffect::~SoundEffect(void)
{
	Free();
}

// Load sound effect.  Specify is sound is a stream and/or looping
void SoundEffect::Load(bool isStream, bool willLoop, char* fileName)
{
	FMOD_MODE mode;
	FMOD_RESULT result;
	willLoop ? mode = FMOD_LOOP_NORMAL : mode = FMOD_DEFAULT;
	if(isStream)
	{
		result = AudioManager::GetInstance()->GetSystem()->
			createStream(fileName, mode, 0, &sound);
	}
	else
	{
		result = AudioManager::GetInstance()->GetSystem()->
			createSound(fileName, mode, 0, &sound);
	}
	// If sound loaded correctly, set filename
	if(result == FMOD_OK)
		fileLocation = fileName;
	// If sound loaded incorrectly, release it and set it to NULL
	else if(sound != NULL)
	{
		sound->release();
		sound = NULL;
	}
}

// Free this object's resources
void SoundEffect::Free(void)
{
	if(sound)
	{
		sound->release();
		sound = NULL;
	}
}