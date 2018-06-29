#pragma once
#include <irrlicht.h>

using namespace irr;
using namespace scene;
using namespace core;

class PowerUpSpawner
{
public:
	void PowerUpDevice(irr::IrrlichtDevice* device);
	void PowerUpSpawn(vector3df postion);

	bool pickUpPower = false;
	IMeshSceneNode* powerupNode;
};

