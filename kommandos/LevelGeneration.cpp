#include <irrlicht.h>
#include "LevelGeneration.h"

using namespace irr;
using namespace core;
using namespace scene;


void LevelGeneration::SetBeginPoint(array<ISceneNode*> nodes)
{

}

void LevelGeneration::PlaceArenas(array<ISceneNode*> nodes)
{
	//vector used for placing the arena's to the left or right of the previous arena.
	vector3df horizontalVector = vector3df(0, 0, 177);

	vector3df nextPosition;

	for (size_t i = 0; i < nodes.size(); i++)
	{
		nextPosition += horizontalVector;
		nodes[i]->setPosition(nextPosition);
	}
}

void LevelGeneration::PlaceDoors(array<ISceneNode*> nodes)
{
}

LevelGeneration::LevelGeneration()
{
}


