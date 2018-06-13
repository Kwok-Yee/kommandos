#include <irrlicht.h>

#pragma once
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

private:
	/// <summary>	rows used in the grid. </summary>
	int rows;
	/// <summary>	columns used in the grid. </summary>
	int columns;
	/// <summary>	start of grid positions X and Z. </summary>
	int startX, startZ;
	/// <summary>	grid with vector3df as type. </summary>
	irr::core::vector3df *grid;

	///-------------------------------------------------------------------------------------------------
	/// <summary> Creates the box obstacles . </summary>
	///-------------------------------------------------------------------------------------------------
	void CreateObjects(irr::IrrlichtDevice* device, irr::scene::ISceneNode* obstacles[] ,int size);

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
	void CheckObstaclePosition(irr::scene::ISceneNode* obstacles[], irr::core::vector3df* usedPositions, int size);

	///-------------------------------------------------------------------------------------------------
	/// <summary> Return a random int that is used in getting an position in the grid</summary>
	///-------------------------------------------------------------------------------------------------
	int RandomPosition();
};

