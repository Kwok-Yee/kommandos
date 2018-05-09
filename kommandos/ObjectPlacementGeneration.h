#include <irrlicht.h>

#pragma once
class ObjectPlacementGeneration
{
public:
	ObjectPlacementGeneration();

	void PlaceObjects(irr::IrrlichtDevice* device);
	void AddObjectsToAvoid(irr::scene::ISceneNode* object);

private:
	int rows;
	int columns;
	int startX, startZ;
	bool unique;
	irr::core::vector3df *grid;
	irr::core::array<irr::scene::ISceneNode*> objectsToAvoid;

	void CreateObjects(irr::IrrlichtDevice* device, irr::scene::ISceneNode* obstacles[] ,int size);
	void CreateGrid();
	void CalculateGrid(irr::scene::ISceneNode* arena);
	void CreateWalls(irr::scene::ISceneManager* smgr);
	int RandomPosition();
	
};

