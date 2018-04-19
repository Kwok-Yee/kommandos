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
	int startX, startZ;
	irr::core::vector3df *grid;
	void CreateGrid();
	
};

