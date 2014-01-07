#pragma once

#ifndef H_SOUNDEFFECT
#define H_SOUNDEFFECT

#include "fmod.h"
#include "fmod.hpp"
#include "fmod_codec.h"
#include "fmod_dsp.h"
#include "fmod_errors.h"
#include "fmod_memoryinfo.h"
#include "fmod_output.h"
#include "AudioManager.h"

class SoundEffect
{
public:
	// Constructor / destructor
	SoundEffect(void);
	~SoundEffect(void);

	// Load / free methods
	void Load(bool isStream, bool willLoop, char* fileName);
	void Free(void);

	operator FMOD::Sound*(void) const	// Cast this object into a Sound*
		{ return sound; }
	FMOD::Sound* GetSound(void)	// Get Sound*
		{ return sound; }

private:
	FMOD::Sound* sound;	// The sound object pointer
	char* fileLocation;	// The location it was loaded from
};

#endif