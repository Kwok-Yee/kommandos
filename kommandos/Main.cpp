/*! \mainpage Kommandos | Irrlicht Engine
*
* \section intro_sec Introduction
*
* Top-down arcade shooter featuring waves of enemies in procedurally generated levels.
* For our main technical goals, we chose to implement: Procedural generation, performance optimization, and controller support.
* This would work perfectly in our game because we could randomly generate the layout of each level, 
* while watching performance with the massive hordes of zombies that will be spawned, the player uses WASD and his mouse for movement and shooting.
* The scope for our game is reachable by our team, while maintaining creative possibilities.
*
* \section install_sec Installation
*
* \subsection step1 Step 1: Pulling
* https://github.com/Kwok-Yee/kommandos
*/

#include <irrlicht.h>
#include "Game.h"

#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(linker, "/subsystem:console /ENTRY:mainCRTStartup")
#endif

int main(int argc, char* argv[])
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
