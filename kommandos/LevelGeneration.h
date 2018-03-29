#pragma once
#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace scene;

class LevelGeneration
{
public:
	//Places arenas in the scene and adds doors with the PlaceDoors method
	void PlaceArenas(ISceneManager* smgr, int maxArenas);	

	LevelGeneration();

private:
	//Places a door in each arena
	void PlaceDoors(ISceneManager* smgr, core::array<ISceneNode*> arenas);
};

