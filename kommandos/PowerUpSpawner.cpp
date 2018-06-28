#include <irrlicht.h>
#include "PowerUpSpawner.h"
#include "collision.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;

IrrlichtDevice* powerUpIDevice;
ISceneManager* powerUpSmgr;
IVideoDriver* powerUpDriver;

Collision* collision;

IMeshSceneNode* powerupSpreadNode;


s32 powerTimer;
s32 powerUptime;



const path FireRate = "../media/Models/powerup";


void PowerUpSpawner::PowerUpDevice(IrrlichtDevice* device)
{
	powerUpIDevice = device;
	powerUpSmgr = device->getSceneManager();
	powerUpDriver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();

	if (device) 
	{
		PowerUpSpawn();
	}
	
}

void PowerUpSpawner::UpdatePowerUp() 
{
	// Work out a frame delta time.
	//const u32 now = powerUpIDevice->getTimer()->getTime();
	//const f32 frameTime = (f32)(now - powerUptime) / 1000.f; // Time in seconds
	//powerUptime = now;

	//if (collision->SceneNodeWithSceneNode(player->PlayerObject, powerupNode))
	//{
	//	//powerupNode->remove;
	//}

	//for removing particle on position
	
	//if (pickUpPower && powerTimer > 0)
	//{
	//	powerTimer -= frameTime;

	//	if (powerTimer <= 0)
	//	{
	//		pickUpPower = false;
	//	}
	//}
}

void PowerUpSpawner::PowerUpSpawn()
{
	IMesh* powerupMesh = powerUpSmgr->getMesh("../media/powerup.3ds");
	IMeshSceneNode* powerupNode = powerUpSmgr->addMeshSceneNode(powerupMesh);
	//powerupNode->setMaterialTexture(0, powerUpDriver->getTexture(FireRate));
	powerupNode->setMaterialFlag(video::EMF_LIGHTING, true);
	powerupNode->setPosition(vector3df(280, 0, 280));
	powerupNode->setScale(vector3df(2, 1, 2));

}
