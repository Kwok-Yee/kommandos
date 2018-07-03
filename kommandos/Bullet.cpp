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
		speed = 300.f;
		damage = 25.f;
		bulletLifeTimer = 150;
		bulletTimer = 15;
		return;
	case BulletMode::rapidFire:
		speed = 350.f;
		bulletLifeTimer = 150;
		bulletTimer = 9;
		return;
	case BulletMode::splitFire:
		speed = 300.f;
		bulletLifeTimer = 150;
		bulletTimer = 13;
		return;
	case BulletMode::rapidSplitFire:
		speed = 300.f;
		bulletLifeTimer = 150;
		bulletTimer = 9;
	}
}

///-------------------------------------------------------------------------------------------------
/// <summary>	Resets this object. </summary>
///-------------------------------------------------------------------------------------------------

void Bullet::Reset()
{
	SetBulletMode(BulletMode::basic);
	bullet->setVisible(false);
	bullet->setPosition(vector3df(0, 50, 0));
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
		position = playerPos;
		delta = vector3df(mousePos.X - playerPos.X, 1.f, mousePos.Z - playerPos.Z);
		length = delta.getLength();
		delta = delta.normalize();

		if ((bulletMode == BulletMode::rapidSplitFire || bulletMode == BulletMode::splitFire) && (spread > 0 || spread < 0))
		{
			perpendicular = vector3df(delta.Z, delta.Y, delta.X);
			offPos = mousePos + (perpendicular * spread * length);
			//recalculate delta based on offset position
			delta = vector3df(offPos.X - playerPos.X, 1.f, offPos.Z - playerPos.Z);
			delta.normalize();
		}
		setOnce = false;
	}

	position += delta * frameDeltaTime * speed;
	position.Y = 1.f;
	bullet->setPosition(position);
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

void Bullet::SetBulletSpread(float s)
{
	spread = s;
}

///-------------------------------------------------------------------------------------------------
/// <summary>	Default constructor. </summary>
///-------------------------------------------------------------------------------------------------

Bullet::Bullet()
{
	bullet = 0;
	SetBulletMode(BulletMode::basic);
}