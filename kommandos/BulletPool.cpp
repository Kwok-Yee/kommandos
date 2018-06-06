#include <irrlicht.h>
#include "BulletPool.h"
#include <iostream>

BulletPool::BulletPool()
{

}

BulletPool* BulletPool::instance = 0;

BulletPool* BulletPool::GetInstance()
{
	if (!instance)
	{
		instance = new BulletPool();
	}
	return instance;
}

Bullet* BulletPool::GetResource()
{
	if (resources.empty())
	{
		return new Bullet();
	}
	else
	{
		Bullet* bullet = resources.front();
		resources.pop_front();
		return bullet;
	}
}

void BulletPool::ReturnResource(Bullet* object)
{
	object->Reset();
	resources.push_back(object);
}