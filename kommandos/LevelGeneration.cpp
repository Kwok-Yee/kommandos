#include <irrlicht.h>
#include "LevelGeneration.h"

using namespace irr;
using namespace core;
using namespace scene;


void LevelGeneration::SetBeginPoint(array<ISceneNode*> nodes)
{

}

void LevelGeneration::SetEndPoint(array<ISceneNode*> nodes)
{
}

void LevelGeneration::PlaceArenas(array<ISceneNode*> nodes)
{
	vector3df lastPostion = vector3df(0, 0, 0);

	for (size_t i = 0; i < nodes.size(); i++)
	{
		printf("test\n");
		lastPostion += vector3df(0,0,200);
		nodes[i]->setPosition(lastPostion);
	}
}

void LevelGeneration::PlaceDoors(array<ISceneNode*> nodes)
{
}

LevelGeneration::LevelGeneration()
{
}


