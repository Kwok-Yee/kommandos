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


// Method for placing objects which uses multiple arrays to create objects and put these in random positions
// The method uses one rule to stop objects from spawning on top of each other.
void ObjectPlacementGeneration::PlaceObjects(IrrlichtDevice* device)
{
	const int amountOfObjects = 10;
	
	ISceneManager* smgr = device->getSceneManager();
	ISceneNode* obstacles[amountOfObjects];
	vector3df* usedPositions;

	CreateWalls(smgr);
	CreateGrid();
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

//Method for keeping track of the positions of objects already in the arena.
void ObjectPlacementGeneration::AddObjectsToAvoid(ISceneNode* object)
{
	objectsToAvoid.push_back(object);
}

//Method for creating the objects with the correct material.
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

// Create a grid on the arena which is based on step size and the amount of rows and columns.
// Reason for using a normal array instead of a 2d array is because of memory use.
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

//Method for getting a random position
int ObjectPlacementGeneration::RandomPosition()
{
	int r = rand() % (rows * columns);
	return r;
}


/*Method for finding the right grid size, the arena size is divided by the obstacle size.
Rows and columns are scaled the so the grid is a bit smaller and the grid is made
by multiplying the rows and columns
*/
void ObjectPlacementGeneration::CalculateGrid(ISceneNode* arena)
{
	vector3df arenaSize = arena->getTransformedBoundingBox().getExtent();
	vector3df obstacleSize = vector3df(10.0f,10.0f,10.0f);

	f32 scale = 0.9;

	rows = (arenaSize.X * scale) / obstacleSize.X;
	columns = (arenaSize.Z * scale) / obstacleSize.Z;

	startX = (arenaSize.X * scale) / 2 - obstacleSize.X;
	startZ = (arenaSize.Z * scale) / 2 - obstacleSize.Z;

	grid = new vector3df[rows * columns];
}

void ObjectPlacementGeneration::CreateWalls(ISceneManager* smgr) {

	IMesh* portalMesh = smgr->getMesh("../media/PortalRed.3ds");
	IMeshSceneNode* portalNode = smgr->addMeshSceneNode(portalMesh);
	portalNode->setPosition(vector3df(75, 0, 0));

	IMesh* planeMesh = smgr->getMesh("../media/ArenaColor.3ds");
	IMeshSceneNode* planeNode = smgr->addMeshSceneNode(planeMesh);
	planeNode->setMaterialFlag(video::EMF_LIGHTING, true);

	IMesh* longWallMeshRight = smgr->getMesh("../media/LongWall.3ds");
	IMeshSceneNode* longWallNodeRight = smgr->addMeshSceneNode(longWallMeshRight);
	longWallNodeRight->setMaterialFlag(EMF_LIGHTING, true);
	longWallNodeRight->setPosition(vector3df(0, 0, -75));

	IMesh* longWallMeshLeft = smgr->getMesh("../media/LongWall.3ds");
	IMeshSceneNode* longWallNodeLeft = smgr->addMeshSceneNode(longWallMeshLeft);
	longWallNodeLeft->setMaterialFlag(EMF_LIGHTING, true);
	longWallNodeLeft->setPosition(vector3df(0, 0, 90));

	IMesh* shortWallMeshUp = smgr->getMesh("../media/ShortWall.3ds");
	IMeshSceneNode* shortWallNodeUp = smgr->addMeshSceneNode(shortWallMeshUp);
	shortWallNodeUp->setMaterialFlag(EMF_LIGHTING, true);
	shortWallNodeUp->setPosition(vector3df(78.5, 0, 0));

	IMesh* shortWallMeshDown = smgr->getMesh("../media/ShortWall.3ds");
	IMeshSceneNode* shortWallNodeDown = smgr->addMeshSceneNode(shortWallMeshDown);
	shortWallNodeDown->setMaterialFlag(EMF_LIGHTING, true);
	shortWallNodeDown->setPosition(vector3df(-93.5, 0, 0));

	CalculateGrid(planeNode);

	/* Add to collision for enemy
	collision.AddStaticToList(longWallNodeRight);
	collision.AddStaticToList(longWallNodeLeft);
	collision.AddStaticToList(shortWallNodeUp);
	collision.AddStaticToList(shortWallNodeDown);*/
}
