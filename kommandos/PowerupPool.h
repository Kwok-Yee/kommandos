#pragma once
#include "Powerup.h"
#include <list>

using namespace std;

class PowerupPool
{
public:

	PowerupPool(const PowerupPool&) = delete;
	PowerupPool& operator=(const PowerupPool&) = delete;
	static PowerupPool* GetInstance();
	Powerup* GetResource();
	void ReturnResource(Powerup* object);

private:

	PowerupPool();
	std::list<Powerup*> resources;
	static PowerupPool* instance;

};

