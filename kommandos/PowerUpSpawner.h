#pragma once
#include <irrlicht.h>

using namespace irr;
using namespace core;

class PowerUpSpawner
{
public:
	void PowerUpDevice(irr::IrrlichtDevice* device);
	void UpdatePowerUp();
	void PowerUpSpawn(irr::IrrlichtDevice* device, vector3df powerUpPosition);

	bool pickUpPower = false;
};
