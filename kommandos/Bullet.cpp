#include <irrlicht.h>
#include "Bullet.h"

using namespace irr;
using namespace scene;

void Bullet::Reset()
{
	bullet = 0;
}

ISceneNode* Bullet::GetBullet()
{
	return bullet;
}

void Bullet::SetBullet(ISceneNode* newBullet)
{
	bullet = newBullet;
}

Bullet::Bullet() 
{
	bullet = 0;
}