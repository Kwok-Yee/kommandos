#pragma once
#include "Bullet.h"
#include <list>

using namespace std;

class BulletPool
{
public:
	BulletPool(const BulletPool&) = delete;
	BulletPool& operator=(const BulletPool&) = delete;
	static BulletPool* GetInstance();
	Bullet* GetResource();
	void ReturnResource(Bullet* object);
private:
	BulletPool();
	std::list<Bullet*> resources;
	static BulletPool* instance;
};
