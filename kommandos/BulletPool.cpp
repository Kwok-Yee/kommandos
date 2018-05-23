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
		cout << "Creating new." << endl;
		return new Bullet();
	}
	else
	{
		cout << "Reusing existing." << endl;
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