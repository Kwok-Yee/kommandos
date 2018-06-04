#include <irrlicht.h>
#include "Bullet.h"
#include <iostream>
using namespace std;

using namespace irr;
using namespace scene;
using namespace core;

void Bullet::Reset()
{
	bullet->setVisible(false);
	bullet->setPosition(vector3df(0, 0, 0));
	setOnce = true;
}

ISceneNode* Bullet::GetBullet()
{
	return bullet;
}

void Bullet::SetBullet(ISceneNode* newBullet)
{
	bullet = newBullet;
}

void Bullet::UpdateBullet(vector3df mousePos, vector3df playerPos, float frameDeltaTime, float speed)
{
	if (setOnce) 
	{
		velocity = playerPos;
		delta = vector3df(mousePos.X - playerPos.X, 1.f, mousePos.Z - playerPos.Z);
		delta.normalize();
		setOnce = false;
	}
	velocity += delta * frameDeltaTime * speed;
	velocity.Y = 1.f;
	bullet->setPosition(velocity);
}

Bullet::Bullet() 
{
	bullet = 0;
}