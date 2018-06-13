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
/// <summary>	Resets this object. </summary>
///-------------------------------------------------------------------------------------------------

void Bullet::Reset()
{
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
/// <summary>	Default constructor. </summary>
///-------------------------------------------------------------------------------------------------

Bullet::Bullet() 
{
	bullet = 0;
	speed = 100.f;
	damage = 25.f;
}