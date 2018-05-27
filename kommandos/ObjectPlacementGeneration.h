#include <irrlicht.h>

#pragma once
class ObjectPlacementGeneration
{
public:
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
	void CheckObstaclePosition(irr::scene::ISceneNode* obstacles[], irr::core::vector3df* usedPositions, int size);
	int RandomPosition();
	
};

