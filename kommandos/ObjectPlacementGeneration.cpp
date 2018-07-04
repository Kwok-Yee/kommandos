///-------------------------------------------------------------------------------------------------
// file:	ObjectPlacementGeneration.cpp
//
// summary: Object placement generation class. In this class multiple objects are defined.
// The class was used for an object placement technique, but was later turned into a general object placement class.
// Default objects are created and placed in this script. The box objects are placed randomly but according to two rules.
// For placing objects PCG style Poisson Disk Sampling was referenced but not fully used.
// This class has 5 methods for the obstacle generation part, 1 default object method and 1 method to create every object.
//
// author:	Nino Thijs
///-------------------------------------------------------------------------------------------------

#include <irrlicht.h>
#include "ObjectPlacementGeneration.h"
#include <iostream>
#include <stdlib.h>
#include <ctime>
#include <algorithm>
#include "Collision.h"
#include "HeatMapManager.h"
#include "Game.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace std;

/// <summary> Collision object. </summary>
Collision coll;

///-------------------------------------------------------------------------------------------------
/// <summary> Method for placing objects which uses multiple arrays to create objects and put these in random positions. </summary>
///-------------------------------------------------------------------------------------------------
void ObjectPlacementGeneration::PlaceObjects(IrrlichtDevice* device)
{
	CreateDefaultObjects(device); // Creates playing field objects.
	CreateGrid(); //Creates the grid which is used for object spawning.
	srand(time(0)); //get a seed for the random so every run is different.
	
	CreateObjects(device, zone1Obstacles); // Adds objects with texture to the obstacles array.
	CreateObjects(device, zone2Obstacles); // Adds objects with texture to the obstacles array.
	CreateObjects(device, zone3Obstacles); // Adds objects with texture to the obstacles array.
	CreateObjects(device, zone4Obstacles); // Adds objects with texture to the obstacles array.

	usedZone1Positions = new vector3df[AMOUNT_OF_OBSTACLES/4]; // initialize the used position array with the amount of objects.
	usedZone2Positions = new vector3df[AMOUNT_OF_OBSTACLES/4]; // initialize the used position array with the amount of objects.
	usedZone3Positions = new vector3df[AMOUNT_OF_OBSTACLES/4]; // initialize the used position array with the amount of objects.
	usedZone4Positions = new vector3df[AMOUNT_OF_OBSTACLES/4]; // initialize the used position array with the amount of objects.

	GenerateNewObjects(HeatMapManager::Zone1);
	GenerateNewObjects(HeatMapManager::Zone2);
	GenerateNewObjects(HeatMapManager::Zone3);
	GenerateNewObjects(HeatMapManager::Zone4);
	/*for (int i = 0; i < AMOUNT_OF_OBSTACLES; i++)
	{
		//set used position to a random position on the grid and set the position of the obstacles to these positions.
		usedPositions[i] = grid[RandomPosition()];
		obstacles[i]->setPosition(usedPositions[i]);
	}

	//Check if obstacles behave to the rule of not spawning on-top of each other.
	CheckObstaclePosition(obstacles, usedPositions, AMOUNT_OF_OBSTACLES);*/

}

	///-------------------------------------------------------------------------------------------------
	///<summary>  Method for creating the objects with the correct material. </summary>
	///-------------------------------------------------------------------------------------------------
void ObjectPlacementGeneration::CreateObjects(IrrlichtDevice* device, ISceneNode* obstacles[])
{
	//Scene manager and driver for creating obstacles
	ISceneManager* smgr = device->getSceneManager();
	IVideoDriver* driver = device->getVideoDriver();

	//The paths for getting the obstacle material.
	const path crateDiffuse = "../media/crate/crate_diffuse.png";
	const path crateNormal = "../media/crate/crate_normal.png";


	//Create obstacles defined by size, add textures and collision.
	for (int i = 0; i < AMOUNT_OF_OBSTACLES/4; i++)
	{
		obstacles[i] = smgr->addCubeSceneNode();
		obstacles[i]->setMaterialTexture(0, driver->getTexture(crateDiffuse));
		obstacles[i]->setMaterialTexture(1, driver->getTexture(crateNormal));
		obstacles[i]->setMaterialFlag(EMF_LIGHTING, true);
		coll.AddStaticToList(obstacles[i]);
	}
}

void ObjectPlacementGeneration::GenerateNewObjects(HeatMapManager::Zone zone) 
{
	//Get device for deleting obstacles
	ISceneManager* smgr = Game::GetInstance()->device->getSceneManager();
	srand(time(0));
	switch (zone) {
	case HeatMapManager::Zone1:
		//generate new objects
		for (int i = 0; i < AMOUNT_OF_OBSTACLES/4; i++)
		{
			//set used position to a random position on the grid and set the position of the obstacles to these positions.
			usedZone1Positions[i] = zone1Grid[RandomPosition()];
			zone1Obstacles[i]->setPosition(usedZone1Positions[i]);
		}
		//Check if obstacles behave to the rule of not spawning on-top of each other.
		CheckObstaclePosition(zone1Obstacles, usedZone1Positions, zone1Grid, AMOUNT_OF_OBSTACLES);
		break;

	case HeatMapManager::Zone2:
		//generate new objects
		for (int i = 0; i < AMOUNT_OF_OBSTACLES / 4; i++)
		{
			//set used position to a random position on the grid and set the position of the obstacles to these positions.
			usedZone2Positions[i] = zone2Grid[RandomPosition()];
			zone2Obstacles[i]->setPosition(usedZone2Positions[i]);
		}
		//Check if obstacles behave to the rule of not spawning on-top of each other.
		CheckObstaclePosition(zone2Obstacles, usedZone2Positions, zone2Grid, AMOUNT_OF_OBSTACLES);
		break;

	case HeatMapManager::Zone3:
		//generate new objects
		for (int i = 0; i < AMOUNT_OF_OBSTACLES / 4; i++)
		{
			//set used position to a random position on the grid and set the position of the obstacles to these positions.
			usedZone3Positions[i] = zone3Grid[RandomPosition()];
			zone3Obstacles[i]->setPosition(usedZone3Positions[i]);
		}
		//Check if obstacles behave to the rule of not spawning on-top of each other.
		CheckObstaclePosition(zone3Obstacles, usedZone3Positions, zone3Grid, AMOUNT_OF_OBSTACLES);
		break;

	case HeatMapManager::Zone4:
		//generate new objects
		for (int i = 0; i < AMOUNT_OF_OBSTACLES/4; i++)
		{
			//set used position to a random position on the grid and set the position of the obstacles to these positions.
			usedZone4Positions[i] = zone4Grid[RandomPosition()];
			zone4Obstacles[i]->setPosition(usedZone4Positions[i]);
		}
		//Check if obstacles behave to the rule of not spawning on-top of each other.
		CheckObstaclePosition(zone4Obstacles, usedZone4Positions, zone4Grid, AMOUNT_OF_OBSTACLES);
		break;

	default:
		break;
	}
}

///-------------------------------------------------------------------------------------------------
/// <summary> Create a grid on the arena which is based on step size and the amount of rows and columns.  </summary>
///-------------------------------------------------------------------------------------------------
void ObjectPlacementGeneration::CreateGrid()
{
	///Reason for using a normal array instead of a 2d array is because of memory use.

	int xStep = 10; //X step for changing position
	int zStep = 10; //Z step for changing position
	const int startY = 0; // Y never changes.
	vector3df startZone1Vector = vector3df(startX, startY, startZ); //Start Vector where the grid for zone 1 begins.
	vector3df startZone2Vector = vector3df(startX, startY, 0); //Start Vector where the grid for zone 2 begins.
	vector3df startZone3Vector = vector3df(0, startY, startZ); //Start Vector where the grid for zone 3 begins.
	vector3df startZone4Vector = vector3df(0, startY, 0); //Start Vector where the grid for zone 4 begins.

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			//Grid array gets its positions like a 2d array.
			zone1Grid[i * rows + j] = startZone1Vector + vector3df(-xStep * j, startY, -(i * zStep));
			zone2Grid[i * rows + j] = startZone2Vector + vector3df(-xStep * j, startY, -(i * zStep));
			zone3Grid[i * rows + j] = startZone3Vector + vector3df(-xStep * j, startY, -(i * zStep));
			zone4Grid[i * rows + j] = startZone4Vector + vector3df(-xStep * j, startY, -(i * zStep));
		}
	}
}

///-------------------------------------------------------------------------------------------------
/// <summary> Method for getting a random position.  </summary>
///-------------------------------------------------------------------------------------------------
int ObjectPlacementGeneration::RandomPosition()
{
	// random int for choosing a position from the grid array.
	int r = rand() % (rows * columns);
	return r;
}

///-------------------------------------------------------------------------------------------------
///  <summary> Method for finding the right grid size, the arena size is divided by the obstacle size. </summary>
///-------------------------------------------------------------------------------------------------
void ObjectPlacementGeneration::CalculateGrid(ISceneNode* arena)
{
	///Rows and columns are scaled the so the grid is a bit smaller and the grid is made by multiplying the rows and columns 

	int resize = 4;
	vector3df arenaSize = arena->getTransformedBoundingBox().getExtent(); // get size of the arena.
	vector3df obstacleSize = vector3df(10,10,10); // set the obstacle size.

	f32 scale = 0.9f; // scale for not using the entire arena as a place where the grid spawns.

	rows = ((arenaSize.X * resize) * scale) / obstacleSize.X; // calculate the amount of rows.
	columns = ((arenaSize.Z * resize) * scale) / obstacleSize.Z;// calculate the amount of columns

	// start X and Z where the grid spawns based on the arena size scaled and divided by 2 and adjusted by the obstacle size
	startX = ((arenaSize.X * resize) * scale) / 2;// -obstacleSize.X;
	startZ = ((arenaSize.Z * resize) * scale) / 2;// -obstacleSize.Z;

	//Because we work with four zones, dived the rows and columns by 2
	rows /= 2;
	columns /= 2;
	zone1Grid = new vector3df[rows * columns]; // initialize grid based on rows times columns.
	zone2Grid = new vector3df[rows * columns]; // initialize grid based on rows times columns.
	zone3Grid = new vector3df[rows * columns]; // initialize grid based on rows times columns.
	zone4Grid = new vector3df[rows * columns]; // initialize grid based on rows times columns.
}

///-------------------------------------------------------------------------------------------------
/// <summary> Method for spawning default objects like walls and the arena floor. </summary>
///-------------------------------------------------------------------------------------------------
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

	HeatMapManager* h = h->GetInstance();
	h->level = planeNode;

	//Add to collision for player and enemy
	coll.AddWallToList(longWallNodeLeft);
	coll.AddWallToList(shortWallNodeUp);
	coll.AddWallToList(longWallNodeRight);
	coll.AddWallToList(shortWallNodeDown);
}

///-------------------------------------------------------------------------------------------------
/// <summary>  Method for checking if an obstacle position is unique. </summary> 
///-------------------------------------------------------------------------------------------------
void ObjectPlacementGeneration::CheckObstaclePosition(ISceneNode* obstacles[], vector3df* usedPositions, vector3df* grid, int size)
{
	///First rule is that objects can't spawn on-top of each other. 
	///Second rule is that objects can't spawn on the default position. 

	vector3df defaultPosition = vector3df(266, 0, 266);

	for (int i = 0; i < size/4 - 1; i++)
	{
		for (int j = i + 1; j < size/4; j++)
		{
			//Check if there are any positions that are the same.
			if (usedPositions[i] == usedPositions[j] || usedPositions[i] == defaultPosition) {
				// if there are positions the same unqiue equals false.
				usedPositions[i] = grid[RandomPosition()];
				obstacles[i]->setPosition(usedPositions[i]);
				CheckObstaclePosition(obstacles, usedPositions, grid, size);
			}
			/*else
			{
				//set obstacle to unique position.
			}*/
		}
	}
}
