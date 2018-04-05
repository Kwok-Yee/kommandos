#pragma once
#include <irrlicht.h>
#include "EnemyBehaviour.h"
#include "player.h"
class EnemySpawner
{
public:
	EnemySpawner(irr::IrrlichtDevice* device, EnemyBehaviour* enemBehaviour, Player* player);
	void UpdateEnemies();
private:
	void Spawn();
};