///-------------------------------------------------------------------------------------------------
// file:	Bullet.cpp
//
// summary:	Implements the bullet class
///-------------------------------------------------------------------------------------------------

#include <irrlicht.h>
#include "Bullet.h"
#include <iostream>
using namespace std;

using namespace irr;
using namespace scene;
using namespace core;

// Split fire mode variables
vector3df leftBulletDelta;
vector3df rightBulletDelta;
vector3df leftBulletVelocity;
vector3df rightBulletVelocity;

///-------------------------------------------------------------------------------------------------
/// <summary>	Set the Bullet Mode. </summary>
///-------------------------------------------------------------------------------------------------

void Bullet::SetBulletMode(BulletMode mode)
{
	bulletMode = mode;
	switch (mode)
	{
	case BulletMode::basic:
		speed = 250.f;
		damage = 25.f;
		bulletLifeTimer = 150;
		bulletTimer = 25;
		return;
	case BulletMode::rapidFire:
		speed = 300.f;
		bulletLifeTimer = 150;
		bulletTimer = 10;
		return;
	case BulletMode::splitFire:
		speed = 250.f;
		bulletLifeTimer = 150;
		bulletTimer = 17;
		return;
	}
}

///-------------------------------------------------------------------------------------------------
/// <summary>	Resets this object. </summary>
///-------------------------------------------------------------------------------------------------

void Bullet::Reset()
{
	SetBulletMode(BulletMode::basic);
	bullet->setVisible(false);
	bullet->setPosition(vector3df(0, 0, 0));
	setOnce = true;
}

///-------------------------------------------------------------------------------------------------
/// <summary>	Gets the bullet. </summary>
///-------------------------------------------------------------------------------------------------

ISceneNode* Bullet::GetBullet()
{
	return bullet;
}

///-------------------------------------------------------------------------------------------------
/// <summary>	Sets a bullet. </summary>
///-------------------------------------------------------------------------------------------------

void Bullet::SetBullet(ISceneNode* newBullet)
{
	bullet = newBullet;
}

///-------------------------------------------------------------------------------------------------
/// <summary>	Gets the damage. </summary>
///-------------------------------------------------------------------------------------------------

float Bullet::GetDamage()
{
	return damage;
}

///-------------------------------------------------------------------------------------------------
/// <summary>	Updates the bullet. </summary>
///-------------------------------------------------------------------------------------------------
void Bullet::UpdateBullet(vector3df mousePos, vector3df playerPos, float frameDeltaTime)
{
	if (bulletLifeTimer > 0)
	{
		bulletLifeTimer -= frameDeltaTime;
	}
	if (setOnce)
	{
		velocity = playerPos;
		if (bulletMode == BulletMode::basic || bulletMode == Bullet::rapidFire)
		{
			delta = vector3df(mousePos.X - playerPos.X, 1.f, mousePos.Z - playerPos.Z);
		}
		if (bulletMode == BulletMode::splitFire)
		{
			delta = vector3df(mousePos.X - playerPos.X, 1.f, mousePos.Z - playerPos.Z + offset);
		}
		delta.normalize();
		setOnce = false;
	}

	velocity += delta * frameDeltaTime * speed;
	velocity.Y = 1.f;
	bullet->setPosition(velocity);
}

///-------------------------------------------------------------------------------------------------
/// <summary>	Sets the bullet speed. </summary>
///-------------------------------------------------------------------------------------------------
void Bullet::SetSpeed(float s)
{
	speed = s;
}

s32 Bullet::GetBulletTimer()
{
	return bulletTimer;
}

s32 Bullet::GetBulletLifeTimer()
{
	return bulletLifeTimer;
}

void Bullet::SetBulletOffset(float o)
{
	offset = o;
}

///-------------------------------------------------------------------------------------------------
/// <summary>	Default constructor. </summary>
///-------------------------------------------------------------------------------------------------

Bullet::Bullet()
{
	bullet = 0;
	SetBulletMode(BulletMode::basic);
}