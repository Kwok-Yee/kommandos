#include <irrlicht.h>
#include "PowerupPool.h"
#include <iostream>

irr::IrrlichtDevice* powPoolDevice;

PowerupPool::PowerupPool(irr::IrrlichtDevice* device) { powPoolDevice = device; }

PowerupPool* PowerupPool::instance = 0;

PowerupPool* PowerupPool::GetInstance(irr::IrrlichtDevice* device)
{
	if (!instance)
	{
		instance = new PowerupPool(device);
	}
	return instance;
}

Powerup* PowerupPool::GetResource()
{
	if (resources.empty())
	{
		return new Powerup(powPoolDevice->getSceneManager());
	}
	else
	{
		Powerup* powerup = resources.front();
		resources.pop_front();
		return powerup;
	}
}

void PowerupPool::ReturnResource(Powerup* object)
{
	object->Reset();
	resources.push_back(object);
}
