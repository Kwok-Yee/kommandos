#include "ObjectPlacementGeneration.h"
#include <iostream>
#include <stdlib.h>
#include <ctime>
#include <algorithm>
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
	vector3df* usedPositions;

	srand(time(0));
	int amountOfObjects = 10;
	usedPositions = new vector3df[amountOfObjects];
	for (int i = 0; i < amountOfObjects; i++)
	{
		obstacles.push_back(smgr->addCubeSceneNode());
		usedPositions[i] = grid[randomPosition()];
		obstacles[i]->setPosition(usedPositions[i]);

	}

	int count = 0;
	for (int i = 0; i < amountOfObjects - 1; i++)
	{
		for (int j = i + 1; j < amountOfObjects; j++)
		{
			if (usedPositions[i] == usedPositions[j]) {
				printf("not unique");
			}
		}
	}
}


ObjectPlacementGeneration::ObjectPlacementGeneration()
{

}

void ObjectPlacementGeneration::AddObjectsToAvoid(ISceneNode* object)
{
	objectsToAvoid.push_back(object);
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

	f32 scale = 0.9;

	rows = (arenaSize.X * scale) / obstacleSize.X;
	columns = (arenaSize.Z * scale) / obstacleSize.Z;

	startX = (arenaSize.X * scale) / 2 - obstacleSize.X;
	startZ = (arenaSize.Z * scale) / 2 - obstacleSize.Z;

	grid = new vector3df[rows * columns];
}
