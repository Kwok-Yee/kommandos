#include "ObjectPlacementGeneration.h"
#include <iostream>
#include <stdlib.h>
#include <ctime>
#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace std;

void ObjectPlacementGeneration::PlaceObjects(IrrlichtDevice* device)
{
	CreateGrid();
	ISceneManager* smgr = device->getSceneManager();
	core::array<ISceneNode*> obstacles;

	srand(time(0));
	int amountOfObjects = 10;
	for (int i = 0; i < amountOfObjects; i++)
	{
		obstacles.push_back(smgr->addCubeSceneNode());
		obstacles[i]->setPosition(grid[randomPosition()]);
	}
}


ObjectPlacementGeneration::ObjectPlacementGeneration()
{
	
}

void ObjectPlacementGeneration::CreateGrid()
{
	int xStep = 10;
	int zStep = 10;
	const int startY = 0;
	vector3df startVector = vector3df(startX, startY, startZ);


	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			grid[i * rows + j] = startVector + vector3df(-xStep * j, startY, -(i * zStep));
		}
	}
}

int ObjectPlacementGeneration::randomPosition()
{
	int r = rand() % (rows * columns);
	return r;
}

void ObjectPlacementGeneration::CalculateGrid(ISceneNode* arena, ISceneNode* obstacle)
{
	vector3df arenaSize = arena->getTransformedBoundingBox().getExtent();
	vector3df obstacleSize = obstacle->getBoundingBox().getExtent();

	rows = arenaSize.X / obstacleSize.X;
	columns = arenaSize.Z / obstacleSize.Z;

	startX = arenaSize.X / 2 - obstacleSize.X;
	startZ = arenaSize.Z / 2 - obstacleSize.Z;

	grid = new vector3df[rows * columns];
}
