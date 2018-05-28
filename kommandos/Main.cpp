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

#define CATCH_CONFIG_MAIN

#include "catch.hpp"

#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(linker, "/subsystem:console /ENTRY:mainCRTStartup")
#endif

int main(int argc, char* argv[])
{

	int result = Catch::Session().run(argc, argv);

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

/* test cases*/

TEST_CASE("TEST 1") {
	REQUIRE(1 + 1 == 4);
}

int sum(int a, int b) {
	return a + b;
}

TEST_CASE("TEST 2") {
	REQUIRE(1 + 1 == 2);
}

TEST_CASE("TEST 3") {
	REQUIRE(sum(10, 20) == 3);
}
