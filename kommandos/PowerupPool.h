#pragma once
#include "Powerup.h"
#include <list>

using namespace std;

class PowerupPool
{
public:

	PowerupPool(const PowerupPool&) = delete;
	PowerupPool& operator=(const PowerupPool&) = delete;
	static PowerupPool* GetInstance(irr::IrrlichtDevice* device);
	Powerup* GetResource();
	void ReturnResource(Powerup* object);

private:

	PowerupPool(irr::IrrlichtDevice* device);
	std::list<Powerup*> resources;
	static PowerupPool* instance;

};

