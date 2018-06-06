
#include "SoundManager.h"
#include <iostream>
#include <irrKlang.h>

using namespace irrklang;

SoundManager::SoundManager()
{
	//mute = false;
	//volume = 100;
	engine = createIrrKlangDevice();
}

SoundManager* SoundManager::instance = 0;

SoundManager* SoundManager::GetInstance()
{
	if (!instance)
	{
		instance = new SoundManager();
	}
	return instance;
}

void SoundManager::PlaySound(const char* sound, bool loop)
{
	engine->play2D(sound, loop);
}