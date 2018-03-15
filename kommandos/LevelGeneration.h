#pragma once
#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace scene;

class LevelGeneration
{
public:
	
	void PlaceArenas(ISceneManager* smgr, ISceneNode* doorMesh, int maxArenas);	

	LevelGeneration();

private:
	void PlaceDoors(core::array<ISceneNode*> arenas, ISceneNode* doorMesh);
	void SetBeginPoint(core::array<ISceneNode*> nodes);
};

