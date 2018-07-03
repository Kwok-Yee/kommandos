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
	//returns the enemy script
	Enemy* GetEnemy(int id);
	static EnemySpawner* GetSpawner();
	//Spawns a specific enemy at the designated spawn position
	void SpawnEnemy(irr::core::vector3df spawnPos, Enemy::EnemyType enemyType, irr::s32 nestAmount = 0);
private:
	//Spawns alot of enemies at random predefined spawn positions
	void Spawn();
	void InitialiseWaveData();
};