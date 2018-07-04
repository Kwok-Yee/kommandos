#pragma once
#include <irrlicht.h>

using namespace irr;
using namespace scene;
using namespace core;

class PowerUpSpawner
{
public:
	void PowerUpDevice(irr::IrrlichtDevice* device);
	//void UpdatePowerUp();
	void PowerUpSpawn(vector3df position);

	bool pickUpPower = false;
	IMeshSceneNode* powerupNode;
};