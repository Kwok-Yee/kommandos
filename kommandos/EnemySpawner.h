#pragma once
#include <irrlicht.h>
#include "EnemyBehaviour.h"

class Player;
class EnemySpawner
{
public:
	EnemySpawner(irr::IrrlichtDevice* device, Player* player);
	void UpdateEnemies();
	irr::core::array<irr::scene::IMeshSceneNode*> getEnemies();
	EnemyBehaviour* getEnemyBehaviour();
	irr::core::array<irr::f32> enemyHealthValues;
private:
	void Spawn();
};