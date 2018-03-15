#include <irrlicht.h>
#include "LevelGeneration.h"

using namespace irr;
using namespace core;
using namespace scene;


void LevelGeneration::SetBeginPoint(array<ISceneNode*> nodes)
{

}

void LevelGeneration::PlaceArenas(ISceneManager* smgr, ISceneNode* doorMesh, int maxArenas)
{
	IMesh* planeMesh = smgr->getMesh("../media/ArenaColor.3ds");
	core::array<ISceneNode*> arenas;
	//vector used for placing the arena's to the left or right of the previous arena.
	vector3df horizontalVector = vector3df(0, 0, 177);

	vector3df nextPosition;

	for (int i = 0; i < maxArenas; i++)
	{
		arenas.push_back(smgr->addMeshSceneNode(planeMesh));
	}

	for (size_t i = 0; i < arenas.size(); i++)
	{
		nextPosition += horizontalVector;
		arenas[i]->setPosition(nextPosition);
	}

	PlaceDoors(arenas, doorMesh);
}

void LevelGeneration::PlaceDoors(core::array<ISceneNode*> arenas, ISceneNode* doorMesh)
{
	const int MAX_DOORS = arenas.size() -2;
	array<ISceneNode*> doors;

	for (size_t i = 0; i < MAX_DOORS; i++)
	{
		doors.push_back(doorMesh);
	}
}

LevelGeneration::LevelGeneration()
{
}


