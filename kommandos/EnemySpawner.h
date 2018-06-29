#pragma once
#include <irrlicht.h>
#include "Enemy.h"
#include <iostream>

class Player;
class EnemySpawner
{
public:
	EnemySpawner(irr::IrrlichtDevice* device, Player* player);
	void UpdateEnemies();
	//returns a list of all active enemy objects in the scene
	irr::core::array<Enemy*> getActiveEnemies();
	//returns the enemyBehaviour script
	Enemy* GetEnemy(int id);
private:
	//Spawns a enemy at a random spaw position
	void Spawn();
};