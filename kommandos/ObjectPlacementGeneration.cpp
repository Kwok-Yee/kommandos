#include <irrlicht.h>
#include "ObjectPlacementGeneration.h"
#include <iostream>
#include <stdlib.h>
#include <ctime>
#include <algorithm>
#include "Collision.h"


using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace std;

Collision coll; // object for collision

// Method for placing objects which uses multiple arrays to create objects and put these in random positions
// The method uses one rule to stop objects from spawning on top of each other.
void ObjectPlacementGeneration::PlaceObjects(IrrlichtDevice* device)
{
	const int amountOfObjects = 300; // set amount of objects
	
	//Get device for creating obstacles
	ISceneManager* smgr = device->getSceneManager();
	ISceneNode* obstacles[amountOfObjects]; // set amount of obstacles for the array.
	vector3df* usedPositions; // Start of array for used positions.

	CreateDefaultObjects(device); // Creates playing field objects.
	CreateGrid(); //Creates the grid which is used for object spawning.
	srand(time(0)); //get a seed for the random so every run is different.
	
	CreateObjects(device, obstacles, amountOfObjects); // Adds objects with texture to the obstacles array.
	usedPositions = new vector3df[amountOfObjects]; // initialize the used position array with the amount of objects.
	for (int i = 0; i < amountOfObjects; i++)
	{
		//set used position to a random position on the grid and set the position of the obstacles to these positions.
		usedPositions[i] = grid[RandomPosition()];
		obstacles[i]->setPosition(usedPositions[i]);
	}

	//First Rule: Objects can't spawn on-top of each other.
	for (int i = 0; i < amountOfObjects - 1; i++)
	{
		for (int j = i + 1; j < amountOfObjects; j++)
		{
			//Check if there are any positions that are the same.
			if (usedPositions[i] == usedPositions[j]) {
				// if there are positions the same unqiue equals false.
				unique = false;
				//While unique is false get a new random position for that used position.
				while (!unique)
				{
					usedPositions[i] = grid[RandomPosition()];
					for (int k = 0; k < amountOfObjects; k++)
					{
						// if unique is still false change it again untill it is unique.
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
								// set position to new unqiue position.
								obstacles[l]->setPosition(usedPositions[l]);
							}
						}
					}
				}
			}
		}
	}

}

//Method for keeping track of the positions of objects already in the arena.
void ObjectPlacementGeneration::AddObjectsToAvoid(ISceneNode* object)
{
	objectsToAvoid.push_back(object);
}

//Method for creating the objects with the correct material.
void ObjectPlacementGeneration::CreateObjects(IrrlichtDevice* device, ISceneNode* obstacles[], int size)
{
	//Scene manager and driver for creating obstacles
	ISceneManager* smgr = device->getSceneManager();
	IVideoDriver* driver = device->getVideoDriver();

	//The paths for getting the obstacle material.
	const path crateDiffuse = "../media/crate/crate_diffuse.png";
	const path crateNormal = "../media/crate/crate_normal.png";


	//Create obstacles defined by size, add textures and collision.
	for (int i = 0; i < size; i++)
	{
		obstacles[i] = smgr->addCubeSceneNode();
		obstacles[i]->setMaterialTexture(0, driver->getTexture(crateDiffuse));
		obstacles[i]->setMaterialTexture(1, driver->getTexture(crateNormal));
		obstacles[i]->setMaterialFlag(EMF_LIGHTING, true);
		coll.AddStaticToList(obstacles[i]);
	}
}

// Create a grid on the arena which is based on step size and the amount of rows and columns.
// Reason for using a normal array instead of a 2d array is because of memory use.
void ObjectPlacementGeneration::CreateGrid()
{
	
	int xStep = 10; //X step for changing position
	int zStep = 10; //Z step for changing position
	const int startY = 0; // Y never changes.
	vector3df startVector = vector3df(startX, startY, startZ); //Start Vector where the grid begins.


	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			//Grid array gets its positions like a 2d array.
			grid[i * rows + j] = startVector + vector3df(-xStep * j, startY, -(i * zStep));
		}
	}
}

//Method for getting a random position
int ObjectPlacementGeneration::RandomPosition()
{
	// random int for choosing a position from the grid array.
	int r = rand() % (rows * columns);
	return r;
}


/*Method for finding the right grid size, the arena size is divided by the obstacle size.
Rows and columns are scaled the so the grid is a bit smaller and the grid is made
by multiplying the rows and columns
*/
void ObjectPlacementGeneration::CalculateGrid(ISceneNode* arena)
{
	int resize = 4;
	vector3df arenaSize = arena->getTransformedBoundingBox().getExtent(); // get size of the arena.
	vector3df obstacleSize = vector3df(10,10,10); // set the obstacle size.

	f32 scale = 0.9; // scale for not using the entire arena as a place where the grid spawns.

	rows = ((arenaSize.X * resize) * scale) / obstacleSize.X; // calculate the amount of rows.
	columns = ((arenaSize.Z * resize) * scale) / obstacleSize.Z;// calculate the amount of columns

															 // start X and Z where the grid spawns based on the arena size scaled and divided by 2 and adjusted by the obstacle size
	startX = ((arenaSize.X * resize) * scale) / 2 - obstacleSize.X;
	startZ = ((arenaSize.Z * resize) * scale) / 2 - obstacleSize.Z;

	grid = new vector3df[rows * columns]; // initialize grid based on rows times columns.
}

//Method for spawning default objects like walls and the arena floor.
void ObjectPlacementGeneration::CreateDefaultObjects(IrrlichtDevice* device) {

	ISceneManager* smgr = device->getSceneManager();
	IVideoDriver* driver = device->getVideoDriver();

	int resizeWall = 4;

	const path floorDiffuse = "../media/ground/ground_diffuse.jpg";

	//Arena mesh with texture placed in the scene.
	IMesh* planeMesh = smgr->getMesh("../media/ArenaFloor.3ds");
	IMeshSceneNode* planeNode = smgr->addMeshSceneNode(planeMesh);
	planeNode->setMaterialTexture(0, driver->getTexture(floorDiffuse));
	planeNode->setMaterialFlag(video::EMF_LIGHTING, true);
	planeNode->setPosition(vector3df(0, 0.1, 0));
	planeNode->setScale(vector3df(3.65f, 1, 4.0f));

	//Walls with texture and position.
	IMesh* longWallMeshRight = smgr->getMesh("../media/LongWall.3ds");
	IMeshSceneNode* longWallNodeRight = smgr->addMeshSceneNode(longWallMeshRight);
	longWallNodeRight->setMaterialFlag(EMF_LIGHTING, true);
	longWallNodeRight->setScale(vector3df(3.65f, 1, 1));
	longWallNodeRight->setPosition(vector3df(0, 0, -86) * resizeWall);

	IMesh* longWallMeshLeft = smgr->getMesh("../media/LongWall.3ds");
	IMeshSceneNode* longWallNodeLeft = smgr->addMeshSceneNode(longWallMeshLeft);
	longWallNodeLeft->setMaterialFlag(EMF_LIGHTING, true);
	longWallNodeLeft->setScale(vector3df(3.65f, 1, 1));
	longWallNodeLeft->setPosition(vector3df(-0.1f, 0, 90) * resizeWall);

	IMesh* shortWallMeshUp = smgr->getMesh("../media/ShortWall.3ds");
	IMeshSceneNode* shortWallNodeUp = smgr->addMeshSceneNode(shortWallMeshUp);
	shortWallNodeUp->setMaterialFlag(EMF_LIGHTING, true);
	shortWallNodeUp->setScale(vector3df(1.0f, 1, 4.27f));
	shortWallNodeUp->setPosition(vector3df(78.5, 0, -0.2) * resizeWall);

	IMesh* shortWallMeshDown = smgr->getMesh("../media/ShortWall.3ds");
	IMeshSceneNode* shortWallNodeDown = smgr->addMeshSceneNode(shortWallMeshDown);
	shortWallNodeDown->setMaterialFlag(EMF_LIGHTING, true);
	shortWallNodeDown->setScale(vector3df(1.0f, 1, 4.27f));
	shortWallNodeDown->setPosition(vector3df(-82.27f, 0, -0.2) * resizeWall);

	const path buildingDiffuse = "../media//BuildingsHighRise.jpg";
	
	IMesh* testMesh = smgr->getMesh("../media/test.3ds");
	IMeshSceneNode* testNode = smgr->addMeshSceneNode(testMesh);
	testNode->setMaterialTexture(0, driver->getTexture(buildingDiffuse));
	testNode->setMaterialFlag(EMF_LIGHTING, false);
	testNode->setScale(vector3df(12.7, 10, 10));
	testNode->setPosition(vector3df(0, -2, -423));

	IMeshSceneNode* testNode2 = smgr->addMeshSceneNode(testMesh);
	testNode2->setMaterialTexture(0, driver->getTexture(buildingDiffuse));
	testNode2->setMaterialFlag(EMF_LIGHTING, false);
	testNode2->setScale(vector3df(14, 10, 10));
	testNode2->setPosition(vector3df(393, -2, 0));
	testNode2->setRotation(vector3df(0, 90, 0));

	IMeshSceneNode* testNode3 = smgr->addMeshSceneNode(testMesh);
	testNode3->setMaterialTexture(0, driver->getTexture(buildingDiffuse));
	testNode3->setMaterialFlag(EMF_LIGHTING, false);
	testNode3->setScale(vector3df(12.7, 10, 10));
	testNode3->setPosition(vector3df(0, -2, 424));

	IMeshSceneNode* testNode4 = smgr->addMeshSceneNode(testMesh);
	testNode4->setMaterialTexture(0, driver->getTexture(buildingDiffuse));
	testNode4->setMaterialFlag(EMF_LIGHTING, false);
	testNode4->setScale(vector3df(14, 10, 10));
	testNode4->setPosition(vector3df(-393, -2, 0));
	testNode4->setRotation(vector3df(0, 90, 0));

	//Calculate the grid using the arena floor
	CalculateGrid(planeNode);

	//Add to collision for player and enemy
	coll.AddStaticToList(longWallNodeRight);
	coll.AddStaticToList(longWallNodeLeft);
	coll.AddStaticToList(shortWallNodeUp);
	coll.AddStaticToList(shortWallNodeDown);
}
