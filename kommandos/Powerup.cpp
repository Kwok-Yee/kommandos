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

Powerup::Powerup(ISceneManager* smgr)
{
	powerup = 0;
	type = PowerupType::base;

	IMesh* powerupMesh = smgr->getMesh("../media/powerup.3ds");
	IMeshSceneNode* powerupNode = smgr->addMeshSceneNode(powerupMesh);
	//powerupNode->setMaterialTexture(0, powerUpDriver->getTexture(FireRate));
	powerupNode->setMaterialFlag(video::EMF_LIGHTING, true);
	powerupNode->setPosition(vector3df(280, 0, 280));
	powerupNode->setScale(vector3df(2, 1, 2));

}

