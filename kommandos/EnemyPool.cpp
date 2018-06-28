#include <irrlicht.h>
#include "EnemyPool.h"
#include <iostream>

irr::IrrlichtDevice* EnemyPoolDevice;
EnemyPool::EnemyPool(irr::IrrlichtDevice* device) 
{
	EnemyPoolDevice = device;
}

EnemyPool* EnemyPool::instance = 0;

EnemyPool* EnemyPool::GetInstance(irr::IrrlichtDevice* device)
{
	if (!instance)
	{
		instance = new EnemyPool(device);
	}
	return instance;
}

Enemy* EnemyPool::GetResource()
{
	if (resources.empty())
	{
		Enemy* enemy = new Enemy(EnemyPoolDevice);
		return enemy;
	}
	else
	{
		Enemy* enemy = resources.front();
		resources.pop_front();
		enemy->GetEnemySceneNode()->setVisible(true);
		return enemy;
	}
}

void EnemyPool::ReturnResource(Enemy* object)
{
	object->Reset();
	resources.push_back(object);
}
