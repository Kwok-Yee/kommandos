#include <irrlicht.h>

#pragma once
class ObjectPlacementGeneration
{
public:
	ObjectPlacementGeneration();

	void PlaceObjects(irr::IrrlichtDevice* device);
	void CalculateGrid(irr::scene::ISceneNode* arena, irr::scene::ISceneNode* obstacle);
	void AddObjectsToAvoid(irr::scene::ISceneNode* object);

private:
	int rows;
	int columns;
	int startX, startZ;
	irr::core::vector3df *grid;
	irr::core::array<irr::scene::ISceneNode*> objectsToAvoid;

	void CreateGrid();
	int randomPosition();
	
};

