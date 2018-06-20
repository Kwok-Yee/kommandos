#include <irrlicht.h>
#include "PowerupPool.h"
#include <iostream>

PowerupPool::PowerupPool() { }

PowerupPool* PowerupPool::instance = 0;

PowerupPool* PowerupPool::GetInstance()
{
	if (!instance)
	{
		instance = new PowerupPool();
	}
	return instance;
}

Powerup* PowerupPool::GetResource()
{
	if (resources.empty())
	{
		return new Powerup();
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
