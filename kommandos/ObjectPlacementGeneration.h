#include <irrlicht.h>
#include "HeatMapManager.h"

#pragma once
#define AMOUNT_OF_OBSTACLES 125 // set amount of objects
class ObjectPlacementGeneration
{
public:
	void PlaceObjects(irr::IrrlichtDevice* device);
	void GenerateNewObjects(HeatMapManager::Zone zone);

private:
	int rows;
	int columns;
	int startX, startZ;
	irr::core::vector3df *zone1Grid, *zone2Grid, *zone3Grid, *zone4Grid;

	irr::scene::ISceneNode *zone1Obstacles[AMOUNT_OF_OBSTACLES], *zone2Obstacles[AMOUNT_OF_OBSTACLES],
		*zone3Obstacles[AMOUNT_OF_OBSTACLES], *zone4Obstacles[AMOUNT_OF_OBSTACLES]; // set amount of obstacles for the array.
	irr::core::vector3df* usedPositions; // Start of array for used positions.

	void CreateObjects(irr::IrrlichtDevice* device, irr::scene::ISceneNode* obstacles[]);
	void CreateGrid();
	void CalculateGrid(irr::scene::ISceneNode* arena);
	void CreateDefaultObjects(irr::IrrlichtDevice* device);
	void CheckObstaclePosition(irr::scene::ISceneNode* obstacles[], irr::core::vector3df* usedPositions, irr::core::vector3df* grid, int size);
	int RandomPosition();
};

