#include <irrlicht.h>

#pragma once

#define AMOUNT_OF_OBJECTS 10 // set amount of objects
class ObjectPlacementGeneration
{
public:
	irr::scene::ISceneNode * obstacles[AMOUNT_OF_OBJECTS]; // set amount of obstacles for the array.
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
	void CreateDefaultObjects(irr::scene::ISceneManager* smgr);
	int RandomPosition();
	
};

