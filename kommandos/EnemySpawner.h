#pragma once
#include <irrlicht.h>
#include "EnemyBehaviour.h"

class Player;
class EnemySpawner
{
public:
	EnemySpawner(irr::IrrlichtDevice* device, Player* player);
	void UpdateEnemies();
	//retruns a list of all enemy objects in the scene
	irr::core::array<irr::scene::IMeshSceneNode*> getEnemies();
	//returns the enemyBehaviour script
	EnemyBehaviour* getEnemyBehaviour();
	irr::core::array<irr::f32> enemyHealthValues;
private:
	//Spawns a enemy at a random spaw position
	void Spawn();
	void InitialiseWaveData();
};