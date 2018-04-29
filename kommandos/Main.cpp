#include <irrlicht.h>
#include "Game.h"

#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(linker, "/subsystem:console /ENTRY:mainCRTStartup")
#endif


int main()
{
	Game* game = new Game();
	game->Start();

	// No device found
	if (!game->device) {
		return 1;
	}
	while (game->device->run())
	{
		game->Update();
		game->Draw();
	}

	game->device->drop();

	return 0;
}