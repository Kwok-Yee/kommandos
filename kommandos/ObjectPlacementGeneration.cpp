#include "ObjectPlacementGeneration.h"
#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace scene;



void ObjectPlacementGeneration::PlaceObjects(IrrlichtDevice* device)
{
	CreateGrid();
	ISceneManager* smgr = device->getSceneManager();
	core::array<ISceneNode*> obstacles;
	for (int i = 0; i < 100; i++)
	{
		obstacles.push_back(smgr->addCubeSceneNode());

	}

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			obstacles[i + j]->setPosition(grid[i][j]);
		}
	}
}

ObjectPlacementGeneration::ObjectPlacementGeneration()
{
	
}.

void ObjectPlacementGeneration::CreateGrid()
{
	int xStep = 10;
	int zStep = 10;
	const int y = 0;
	const int row = 10;
	const int column = 10;
	vector3df startVector = vector3df(80, y, 75);

	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < column; j++)
		{
			grid[i][j] = startVector;
			startVector += vector3df(-(i * xStep), y, -zStep);
		}
	}
}