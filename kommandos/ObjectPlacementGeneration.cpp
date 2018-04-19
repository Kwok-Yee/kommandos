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
	core::array<vector3df> positions;

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			positions.push_back(grid[i][j]);
		}
	}

	for (int i = 0; i < rows * columns; i++)
	{
		obstacles.push_back(smgr->addCubeSceneNode());
		obstacles[i]->setPosition(positions[i]);
	}
}


ObjectPlacementGeneration::ObjectPlacementGeneration()
{
	
}

void ObjectPlacementGeneration::CreateGrid()
{
	int xStep = 12;
	int zStep = 12;
	const int y = 0;
	vector3df startVector = vector3df(80, y, 75);

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			grid[i][j] = startVector + vector3df(-xStep * j, y, -(i * zStep));
		}
	}
}

void ObjectPlacementGeneration::CalculateGrid(ISceneNode* arena, ISceneNode* obstacle)
{
	vector3df arenaSize = arena->getTransformedBoundingBox().getExtent();
	vector3df obstacleSize = obstacle->getBoundingBox().getExtent();

	rows = arenaSize.X / obstacleSize.X;
	columns = arenaSize.Z / obstacleSize.Z;

	grid[rows][columns];
}
