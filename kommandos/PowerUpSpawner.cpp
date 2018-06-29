#include <irrlicht.h>
#include "PowerUpSpawner.h"
#include "PowerupPool.h"
#include "Powerup.h"
#include "Collision.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;

IrrlichtDevice* powerUpIDevice;
ISceneManager* powerUpSmgr;
IVideoDriver* powerUpDriver;

PowerupPool* pool;
Powerup* powerup;
Collision powCol;

s32 powerTimer;
s32 powerUptime;


void PowerUpSpawner::PowerUpDevice(IrrlichtDevice* device)
{
	powerUpIDevice = device;
	powerUpSmgr = device->getSceneManager();
	powerUpDriver = device->getVideoDriver();

	pool = pool->GetInstance(powerUpIDevice);
}

void PowerUpSpawner::PowerUpSpawn(vector3df position)
{
	powerup = pool->GetResource();
	powerup->SetPowerupType(1);
	powCol.AddPowerupToList(powerup);

	IMeshSceneNode* powerupNode = powerUpSmgr->addMeshSceneNode(powerUpSmgr->getMesh("../media/powerup.3ds"));
	if (powerupNode)
	{
		powerupNode->setMaterialFlag(video::EMF_LIGHTING, false);

		powerupNode->setScale(vector3df(2, 1, 2));
		//powerup->setMaterialTexture(0, powerupDriver->getTexture(FireRate));
		powerupNode->setVisible(true);
		powerupNode->setPosition(position);
	}
	powerup->SetPowerup(powerupNode);
}
