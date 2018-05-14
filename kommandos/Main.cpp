/*! \mainpage My Personal Index Page
*
* \section intro_sec Introduction
*
* This is the introduction.
*
* \section install_sec Installation
*
* \subsection step1 Step 1: Opening the box
*
* etc...
*/


#include <irrlicht.h>
#include "Game.h"

#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(linker, "/subsystem:console /ENTRY:mainCRTStartup")
#endif

int main()
{
	// Get the game's singleton instance
	Game* game = game->GetInstance();
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