#include "ObjectPlacementGeneration.h"
#include <irrlicht.h>
#include <iostream>

using namespace irr;
using namespace core;
using namespace scene;
using namespace std;



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
			positions.push_back(grid[i * rows + j]);
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
	int xStep = 10;
	int zStep = 10;
	const int y = 0;
	vector3df startVector = vector3df(startX, y, startZ);


	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			grid[i * rows + j] = startVector + vector3df(-xStep * j, y, -(i * zStep));
		}
	}
}

void ObjectPlacementGeneration::CalculateGrid(ISceneNode* arena, ISceneNode* obstacle)
{
	vector3df arenaSize = arena->getTransformedBoundingBox().getExtent();
	vector3df obstacleSize = obstacle->getBoundingBox().getExtent();

	rows = arenaSize.X / obstacleSize.X;
	columns = arenaSize.Z / obstacleSize.Z;

	startX = arenaSize.X / 2;
	startZ = arenaSize.Z / 2;
	cout << startX << endl;

	grid = new vector3df[rows * columns];
}
