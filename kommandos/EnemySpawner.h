#pragma once
#include <irrlicht.h>
#include "Enemy.h"
#include <iostream>

using namespace irr;
using namespace core;
using namespace scene;
using namespace std;

extern u32 currentWave;
extern u32 waveCooldown;
extern bool waveChangeUI;

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
	void SpawnMathroskaMinion(irr::core::vector3df spawnPos, Enemy::EnemyType enemyType, irr::s32 nestAmount = 0);
	
private:
	void NextWave();
	//Spawns alot of enemies at random predefined spawn positions
	void Spawn();
	void InitialiseWaveData();
};