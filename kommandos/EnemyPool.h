#pragma once
#include "Enemy.h"
#include <list>

using namespace std;

class EnemyPool
{
public:

	EnemyPool(const EnemyPool&) = delete;
	EnemyPool& operator=(const EnemyPool&) = delete;
	static EnemyPool* GetInstance(irr::IrrlichtDevice* device);
	Enemy* GetResource();
	void ReturnResource(Enemy* object);

private:

	EnemyPool(irr::IrrlichtDevice* device);
	std::list<Enemy*> resources;
	static EnemyPool* instance;

};


