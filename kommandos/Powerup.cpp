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
using namespace io;

const path FireRate = "../media/Models/powerup";

IrrlichtDevice* powerupIDevice;
irr::video::IVideoDriver* powerupDriver;
ISceneManager* powerupSmgr;

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
		Powerup::type = Powerup::PowerupType::health;
		powerup->setMaterialTexture(0, powerupDriver->getTexture("../media/Textures/Health_Cross.png"));
		break;
	case 1:
		Powerup::type = Powerup::PowerupType::firerate;
		powerup->setMaterialTexture(0, powerupDriver->getTexture("../media/Textures/rapidfire_texture.png"));
		break;
	case 2:
		Powerup::type = Powerup::PowerupType::splitshot;
		powerup->setMaterialTexture(0, powerupDriver->getTexture("../media/Textures/splitshot_texture.png"));
		break;
	default:
		Powerup::type = Powerup::PowerupType::base;
		break;
	}
}

s32 Powerup::GetPowerupType()
{
	return type;
}

Powerup::Powerup(IrrlichtDevice* device)
{
	powerupIDevice = device;
	powerupDriver = powerupIDevice->getVideoDriver();
	powerupSmgr = powerupIDevice->getSceneManager();

	powerup = 0;
	SetPowerupType(PowerupType::base);
}

