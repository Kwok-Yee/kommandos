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
	const int amountOfObjects = 125; // set amount of objects
	
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

	//Check if obstacles behave to the rule of not spawning on-top of each other.
	CheckObstaclePosition(obstacles, usedPositions, amountOfObjects);

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


	// Below here the building models are defined and placed.
	const path buildingDiffuse = "../media//BuildingsHighRise.jpg";
	IMesh* buildingMesh = smgr->getMesh("../media/Building.3ds");
	f32 yPos = -5.0f;

	IMeshSceneNode* rightBuildingNode = smgr->addMeshSceneNode(buildingMesh);
	rightBuildingNode->setMaterialTexture(0, driver->getTexture(buildingDiffuse));
	rightBuildingNode->setMaterialFlag(EMF_LIGHTING, false);
	rightBuildingNode->setScale(vector3df(12.7, 10, 10));
	rightBuildingNode->setPosition(vector3df(0, yPos, 424));

	IMeshSceneNode* leftBuildingNode = smgr->addMeshSceneNode(buildingMesh);
	leftBuildingNode->setMaterialTexture(0, driver->getTexture(buildingDiffuse));
	leftBuildingNode->setMaterialFlag(EMF_LIGHTING, false);
	leftBuildingNode->setScale(vector3df(12.7, 10, 10));
	leftBuildingNode->setPosition(vector3df(0, yPos, -423));

	IMeshSceneNode* topBuildingNode = smgr->addMeshSceneNode(buildingMesh);
	topBuildingNode->setMaterialTexture(0, driver->getTexture(buildingDiffuse));
	topBuildingNode->setMaterialFlag(EMF_LIGHTING, false);
	topBuildingNode->setScale(vector3df(14, 10, 10));
	topBuildingNode->setPosition(vector3df(393, yPos, 0));
	topBuildingNode->setRotation(vector3df(0, 90, 0));

	IMeshSceneNode* bottomBuildingNode = smgr->addMeshSceneNode(buildingMesh);
	bottomBuildingNode->setMaterialTexture(0, driver->getTexture(buildingDiffuse));
	bottomBuildingNode->setMaterialFlag(EMF_LIGHTING, false);
	bottomBuildingNode->setScale(vector3df(14, 10, 10));
	bottomBuildingNode->setPosition(vector3df(-393, yPos, 0));
	bottomBuildingNode->setRotation(vector3df(0, 90, 0));

	//Calculate the grid using the arena floor
	CalculateGrid(planeNode);

	//Add to collision for player and enemy
	coll.AddWallToList(longWallNodeLeft);
	coll.AddWallToList(shortWallNodeUp);
	coll.AddWallToList(longWallNodeRight);
	coll.AddWallToList(shortWallNodeDown);
}

//Method for checking if an obstacle position is unique.
void ObjectPlacementGeneration::CheckObstaclePosition(ISceneNode * obstacles[], vector3df* usedPositions, int size)
{
	vector3df defaultPosition = vector3df(266, 0, 266);

	for (int i = 0; i < size - 1; i++)
	{
		for (int j = i + 1; j < size; j++)
		{
			//Check if there are any positions that are the same.
			if (usedPositions[i] == usedPositions[j] || usedPositions[i] == defaultPosition) {
				// if there are positions the same unqiue equals false.
				usedPositions[i] = grid[RandomPosition()];
				CheckObstaclePosition(obstacles, usedPositions, size);
			}
			else
			{
				//set obstacle to unique position.
				obstacles[i]->setPosition(usedPositions[i]);
			}
		}
	}
}
