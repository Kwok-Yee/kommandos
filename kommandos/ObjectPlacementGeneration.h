#include <irrlicht.h>

#pragma once
class ObjectPlacementGeneration
{
public:
	void PlaceObjects(irr::IrrlichtDevice* device);
	ObjectPlacementGeneration();

private:
	irr::core::vector3df grid[10][10];
	void CreateGrid();
	void ObjectToGrid();
	irr::core::vector2df RandomPointAround();
};

