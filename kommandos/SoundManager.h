#include <irrlicht.h>
#include <irrKlang.h>
#include <iostream>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace std;
using namespace io;

//Singleton SoundManager that call upon the IrrKlang library
class SoundManager
{
	public:
		SoundManager(const SoundManager&) = delete;
		SoundManager& operator=(const SoundManager&) = delete;
		static SoundManager* GetInstance();
		void PlaySound(const char* sound, bool loop);
		irrklang::ISoundEngine* engine;
	private:
	static SoundManager* instance;
	SoundManager();
};
