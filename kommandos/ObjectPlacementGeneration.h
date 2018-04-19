#include <irrlicht.h>

#pragma once
class ObjectPlacementGeneration
{
public:
	void PlaceObjects(irr::IrrlichtDevice* device);
	void CalculateGrid(irr::scene::ISceneNode* arena, irr::scene::ISceneNode* obstacle);
	ObjectPlacementGeneration();

private:
	int rows;
	int columns;
	irr::core::vector3df grid[1][1];
	void CreateGrid();
	
};

