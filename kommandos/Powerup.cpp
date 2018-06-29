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

ISceneNode* Powerup::GetNode()
{
	return powerup;
}

void Powerup::SetPowerup(ISceneNode* newPowerup)
{
	powerup = newPowerup;
}

void Powerup::SetPowerupType(s32 powType)
{
	switch (powType)
	{
	case 0:
		Powerup::type = Powerup::PowerupType::base;
		break;
	case 1:
		Powerup::type = Powerup::PowerupType::firerate;
		break;
	case 2:
		Powerup::type = Powerup::PowerupType::splitshot;
		break;
	}
	
}

s32 Powerup::GetPowerupType()
{
	return type;
}

Powerup::Powerup()
{
	powerup = 0;
	type = PowerupType::base;
}
