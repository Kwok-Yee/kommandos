#include <irrlicht.h>

#pragma once
class ObjectPlacementGeneration
{
public:
	void PlaceObjects(irr::IrrlichtDevice* device);

private:
	int rows;
	int columns;
	int startX, startZ;
	irr::core::vector3df *grid;

	void CreateObjects(irr::IrrlichtDevice* device, irr::scene::ISceneNode* obstacles[] ,int size);
	void CreateGrid();
	void CalculateGrid(irr::scene::ISceneNode* arena);
	void CreateDefaultObjects(irr::IrrlichtDevice* device);
	void CheckObstaclePosition(irr::scene::ISceneNode* obstacles[], irr::core::vector3df* usedPositions, int size);
	int RandomPosition();
};

