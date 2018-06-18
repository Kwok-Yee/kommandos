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
		bulletTimer = 30;
		return;
	case BulletMode::rapidFire:
		speed = 300.f;
		bulletTimer = 10;
		return;
	case BulletMode::splitFire:
		speed = 250.f;
		bulletTimer = 20;
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

///-------------------------------------------------------------------------------------------------
/// <summary>	Default constructor. </summary>
///-------------------------------------------------------------------------------------------------

Bullet::Bullet()
{
	bullet = 0;
	SetBulletMode(BulletMode::basic);
}