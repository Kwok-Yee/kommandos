///-------------------------------------------------------------------------------------------------
// file:	Powerup.cpp
//
// summary:	Implements the powerup class
///-------------------------------------------------------------------------------------------------

#include <irrlicht.h>
#include "Powerup.h"
#include <iostream>

using namespace std;
using namespace irr;
using namespace scene;
using namespace core;

void Powerup::Reset()
{
	powerup->setVisible(false);
	powerup->setPosition(vector3df(0, 0, 0));
}

ISceneNode* Powerup::GetPowerup()
{
	return powerup;
}

void Powerup::SetPowerup(ISceneNode* newPowerup)
{
	powerup = newPowerup;
}

s32 Powerup::GetPowerupType()
{
	return type;
}

Powerup::Powerup()
{
	powerup = 0;
	type = 0;
}
