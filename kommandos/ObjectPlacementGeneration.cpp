#include "ObjectPlacementGeneration.h"
#include <iostream>
#include <stdlib.h>
#include <ctime>
#include <algorithm>
#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace std;

void ObjectPlacementGeneration::PlaceObjects(IrrlichtDevice* device)
{
	CreateGrid();
	const int amountOfObjects = 10;
	
	ISceneManager* smgr = device->getSceneManager();
	ISceneNode* obstacles[amountOfObjects];
	vector3df* usedPositions;

	srand(time(0));
	
	CreateObjects(device, obstacles, amountOfObjects);
	usedPositions = new vector3df[amountOfObjects];
	for (int i = 0; i < amountOfObjects; i++)
	{
		usedPositions[i] = grid[RandomPosition()];
		obstacles[i]->setPosition(usedPositions[i]);
	}

	//Code for not spawning on top of each other.
	for (int i = 0; i < amountOfObjects - 1; i++)
	{
		for (int j = i + 1; j < amountOfObjects; j++)
		{
			if (usedPositions[i] == usedPositions[j]) {
				unique = false;
				while (!unique)
				{
					usedPositions[i] = grid[RandomPosition()];
					for (int k = 0; k < amountOfObjects; k++)
					{
						if (usedPositions[i] == usedPositions[k]) {
							unique = false;
							usedPositions[i] = grid[RandomPosition()];
							printf("changed");
						}
						else
						{
							unique = true;
							for (int l = 0; l < amountOfObjects; l++)
							{
								obstacles[l]->setPosition(usedPositions[l]);
							}
						}
					}
				}
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

void ObjectPlacementGeneration::CreateObjects(IrrlichtDevice* device, ISceneNode* obstacles[], int size)
{
	ISceneManager* smgr = device->getSceneManager();
	IVideoDriver* driver = device->getVideoDriver();

	const path crateDiffuse = "../media/crate/crate_diffuse.png";
	const path crateNormal = "../media/crate/crate_normal.png";

	for (int i = 0; i < size; i++)
	{
		obstacles[i] = smgr->addCubeSceneNode();
		obstacles[i]->setMaterialTexture(0, driver->getTexture(crateDiffuse));
		obstacles[i]->setMaterialTexture(1, driver->getTexture(crateNormal));
		obstacles[i]->setMaterialFlag(EMF_LIGHTING, true);
	}
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

int ObjectPlacementGeneration::RandomPosition()
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
