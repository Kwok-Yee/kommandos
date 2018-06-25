#include <irrlicht.h>
#include "PowerUpSpawner.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;

IrrlichtDevice* powerUpIDevice;
ISceneManager* powerUpSmgr;
IVideoDriver* powerUpDriver;

s32 powerTimer;
s32 time;

const path FireRate = "../media/Models/powerup";


void PowerUpSpawner::PowerUpDevice(IrrlichtDevice* device)
{
	powerUpIDevice = device;
	powerUpSmgr = device->getSceneManager();
	powerUpDriver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();

	IMesh* powerupMesh = smgr->getMesh("../media/powerup.3ds");
	IMeshSceneNode* powerupNode = smgr->addMeshSceneNode(powerupMesh);
	//powerupNode->setMaterialTexture(0, powerUpDriver->getTexture(FireRate));
	powerupNode->setMaterialFlag(video::EMF_LIGHTING, true);
	powerupNode->setPosition(vector3df(266, 0, 266));
	powerupNode->setScale(vector3df(2, 1, 2));
}

void PowerUpSpawner::UpdatePowerUp() 
{
	// Work out a frame delta time.
	const u32 now = powerUpIDevice->getTimer()->getTime();
	const f32 frameTime = (f32)(now - time) / 1000.f; // Time in seconds
	time = now;

	//for removing particle on position
	if (pickUpPower && powerTimer > 0)
	{
		powerTimer -= frameTime;

		if (powerTimer <= 0)
		{
			pickUpPower = false;
		}
	}
}

void PowerUpSpawner::PowerUpSpawn()
{
	IMesh* powerupMesh = powerUpSmgr->getMesh("../media/powerup.3ds");
	IMeshSceneNode* powerupNode = powerUpSmgr->addMeshSceneNode(powerupMesh);
	//powerupNode->setMaterialTexture(0, powerUpDriver->getTexture(FireRate));
	powerupNode->setMaterialFlag(video::EMF_LIGHTING, true);
	powerupNode->setPosition(vector3df(266, 0, 266));
	powerupNode->setScale(vector3df(2, 1, 2));
}
