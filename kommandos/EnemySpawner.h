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
	//returns the enemyBehaviour script
	Enemy* GetEnemy(int id);
	
private:
	void NextWave();
	void Spawn();
	void InitialiseWaveData();
};