#include <irrlicht.h>
#include "PowerUpSpawner.h"
#include <iostream>
//#include "collision.h"
//#include "player.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace std;

IrrlichtDevice* powerUpIDevice;
ISceneManager* powerUpSmgr;
IVideoDriver* powerUpDriver;

//Player* player;
//Collision* collision;

s32 powerTimer;
s32 time_PowerUpSpawner;

long powerUpChance;
const path FireRate = "../media/Models/powerup";
extern int killedEnemies;

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
	const f32 frameTime = (f32)(now - time_PowerUpSpawner) / 1000.f; // Time in seconds
	time_PowerUpSpawner = now;

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

void PowerUpSpawner::PowerUpSpawn(IrrlichtDevice* device, vector3df powerUpPosition)
{
	/*if (collision->CollidesWithStaticObjects(player->getPlayerObject))
	{

	}*/

	ISceneManager* smgr = device->getSceneManager();
	IVideoDriver* driver = device->getVideoDriver();

	powerUpChance = rand() % 100;
	if (powerUpChance >= 90)
	{
		printf("Rapidfire");
		IMeshSceneNode* powerupNode = smgr->addCubeSceneNode();
		powerupNode->setMaterialFlag(video::EMF_LIGHTING, true);
		powerupNode->setPosition(vector3df(powerUpPosition));
		powerupNode->setScale(vector3df(0.125, 0.125, 0.125));
	}
	if (killedEnemies % 25 == 0) {
		if (powerUpChance < 90 && powerUpChance >= 40)
		{
		printf("Twinshot");
		}
		IMeshSceneNode* powerupNode = smgr->addSphereSceneNode();
		powerupNode->setMaterialFlag(video::EMF_LIGHTING, true);
		powerupNode->setPosition(vector3df(powerUpPosition));
		powerupNode->setScale(vector3df(0.125, 0.125, 0.125));
	}

	//IMesh* powerupMesh = powerUpSmgr->getMesh("../media/powerup.3ds");
	//IMeshSceneNode* powerupNode = powerUpSmgr->addCubeSceneNode(powerupMesh);
	//IMesh* powerUpMesh = smgr->getMesh(gameOver);
	//IMeshSceneNode* gameOverNode = smgr->addMeshSceneNode(gameover);
	//powerupNode->setMaterialTexture(0, powerUpDriver->getTexture(FireRate));

}
