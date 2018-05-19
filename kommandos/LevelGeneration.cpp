#include <irrlicht.h>
#include "LevelGeneration.h"

using namespace irr;
using namespace core;
using namespace scene;

void LevelGeneration::PlaceArenas(ISceneManager* smgr, int maxArenas)
{
	IMesh* planeMesh = smgr->getMesh("../media/ArenaColor.3ds");
	core::array<ISceneNode*> arenas;
	vector3df nextPosition;

	//vector used for placing the arena's to the left or right of the previous arena.
	vector3df horizontalVector = vector3df(0, 0, 177);

	for (int i = 0; i < maxArenas; i++)
	{
		arenas.push_back(smgr->addMeshSceneNode(planeMesh));
		nextPosition += horizontalVector;
		arenas[i]->setPosition(nextPosition);
	}

	PlaceDoors(smgr, arenas);
}

void LevelGeneration::PlaceDoors(ISceneManager* smgr, core::array<ISceneNode*> arenas)
{
	array<ISceneNode*> doors;

	//Position for placing door at left side of the arena.
	vector3df doorPosition = vector3df(0, 0, 80);

	for (size_t i = 0; i < arenas.size(); i++)
	{
		doors.push_back(smgr->addCubeSceneNode());
		doors[i]->setPosition(arenas[i]->getPosition() + doorPosition);
		if (i == arenas.size() - 1) doors[i]->setPosition(arenas[i]->getPosition() - doorPosition);
	}
}

LevelGeneration::LevelGeneration()
{
}


