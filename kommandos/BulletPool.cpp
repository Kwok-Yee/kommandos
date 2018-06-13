///-------------------------------------------------------------------------------------------------
// file:	BulletPool.cpp
//
// summary:	Implements the bullet pool class
///-------------------------------------------------------------------------------------------------

#include <irrlicht.h>
#include "BulletPool.h"
#include <iostream>

///-------------------------------------------------------------------------------------------------
/// <summary>	Default constructor. </summary>
///-------------------------------------------------------------------------------------------------

BulletPool::BulletPool()
{

}

BulletPool* BulletPool::instance = 0;

///-------------------------------------------------------------------------------------------------
/// <summary>	Gets the instance. </summary>
///-------------------------------------------------------------------------------------------------

BulletPool* BulletPool::GetInstance()
{
	if (!instance)
	{
		instance = new BulletPool();
	}
	return instance;
}

///-------------------------------------------------------------------------------------------------
/// <summary>	Gets the resource. </summary>
///-------------------------------------------------------------------------------------------------

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

///-------------------------------------------------------------------------------------------------
/// <summary>	Resets the object and returns the resource back to the resources. </summary>
///-------------------------------------------------------------------------------------------------

void BulletPool::ReturnResource(Bullet* object)
{
	object->Reset();
	resources.push_back(object);
}