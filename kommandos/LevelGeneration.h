#pragma once
#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace scene;

class LevelGeneration
{
public:
	
	void PlaceArenas(array<ISceneNode*> nodes);
	void PlaceDoors(array<ISceneNode*> nodes);

	LevelGeneration();

private:
	void SetBeginPoint(array<ISceneNode*> nodes);
	void SetEndPoint(array<ISceneNode*> nodes);
};

