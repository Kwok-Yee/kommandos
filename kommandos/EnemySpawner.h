#pragma once
#include <irrlicht.h>
class EnemySpawner
{
public:
	EnemySpawner(irr::IrrlichtDevice* device);

private:
	void Spawn();
};