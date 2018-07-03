#include <irrlicht.h>
#include "HeatMapManager.h"

#pragma once
#define AMOUNT_OF_OBSTACLES 125 // set amount of objects
///-------------------------------------------------------------------------------------------------
/// <summary>	Object placment generation for placing obstacles and default objects. </summary>
///-------------------------------------------------------------------------------------------------
class ObjectPlacementGeneration
{
public:
	///-------------------------------------------------------------------------------------------------
	/// <summary>	Places all objects in the static object in the scene. </summary>
	///-------------------------------------------------------------------------------------------------
	void PlaceObjects(irr::IrrlichtDevice* device);
	void GenerateNewObjects(HeatMapManager::Zone zone);

private:
	/// <summary>	rows used in the grid. </summary>
	int rows;
	/// <summary>	columns used in the grid. </summary>
	int columns;
	/// <summary>	start of grid positions X and Z. </summary>
	int startX, startZ;
	irr::core::vector3df *zone1Grid, *zone2Grid, *zone3Grid, *zone4Grid;

	irr::scene::ISceneNode *zone1Obstacles[AMOUNT_OF_OBSTACLES/4], *zone2Obstacles[AMOUNT_OF_OBSTACLES/4],
		*zone3Obstacles[AMOUNT_OF_OBSTACLES/4], *zone4Obstacles[AMOUNT_OF_OBSTACLES/4]; // set amount of obstacles for the array.
	irr::core::vector3df *usedZone1Positions, *usedZone2Positions, *usedZone3Positions, *usedZone4Positions; // Start of array for used positions.


	///-------------------------------------------------------------------------------------------------
	/// <summary> Creates the box obstacles . </summary>
	///-------------------------------------------------------------------------------------------------
	void CreateObjects(irr::IrrlichtDevice* device, irr::scene::ISceneNode* obstacles[]);

	///-------------------------------------------------------------------------------------------------
	/// <summary> Creates the grid on which the obstacles are spawned </summary>
	///-------------------------------------------------------------------------------------------------
	void CreateGrid();

	///-------------------------------------------------------------------------------------------------
	/// <summary> Calculates the grid size	 </summary>
	///-------------------------------------------------------------------------------------------------
	void CalculateGrid(irr::scene::ISceneNode* arena);

	///-------------------------------------------------------------------------------------------------
	/// <summary> Creates the default static objects.  </summary>
	///-------------------------------------------------------------------------------------------------
	void CreateDefaultObjects(irr::IrrlichtDevice* device);

	///-------------------------------------------------------------------------------------------------
	/// <summary> Checks if where a box obstacle spawns.  </summary>
	///-------------------------------------------------------------------------------------------------
	void CheckObstaclePosition(irr::scene::ISceneNode* obstacles[], irr::core::vector3df* usedPositions, irr::core::vector3df* grid, int size);

	///-------------------------------------------------------------------------------------------------
	/// <summary> Return a random int that is used in getting an position in the grid</summary>
	///-------------------------------------------------------------------------------------------------
	int RandomPosition();
};

