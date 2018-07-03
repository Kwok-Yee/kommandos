#include <irrlicht.h>
#include "PowerUpSpawner.h"
#include "PowerupPool.h"
#include "Powerup.h"
#include "Collision.h"
#include <iostream>


using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace std;

IrrlichtDevice* powerUpIDevice;
ISceneManager* powerUpSmgr;
IVideoDriver* powerUpDriver;

//s32 time_PowerUpSpawner;
long powerUpChance;
const path FireRate = "../media/Models/powerup";
extern int killedEnemies;

PowerupPool* pool;
Powerup* powerup;
Collision powCol;
//s32 powerUptime;


void PowerUpSpawner::PowerUpDevice(IrrlichtDevice* device)
{
	powerUpIDevice = device;
	powerUpSmgr = device->getSceneManager();
	powerUpDriver = device->getVideoDriver();

	pool = pool->GetInstance(powerUpIDevice);
}

void PowerUpSpawner::PowerUpSpawn(vector3df powerUpPosition)
{
	powerUpChance = rand() % 100;
	if (powerUpChance >= 90)
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
			powerupNode->setPosition(powerUpPosition);
		}
		powerup->SetPowerup(powerupNode);
	}
	if (killedEnemies % 25 == 0)
	{
		if (powerUpChance < 90 && powerUpChance >= 40)
		{
			powerup = pool->GetResource();
			powerup->SetPowerupType(2);
			powCol.AddPowerupToList(powerup);

			IMeshSceneNode* powerupNode = powerUpSmgr->addMeshSceneNode(powerUpSmgr->getMesh("../media/powerup.3ds"));
			if (powerupNode)
			{
				powerupNode->setMaterialFlag(video::EMF_LIGHTING, false);

				powerupNode->setScale(vector3df(2, 1, 2));
				//powerup->setMaterialTexture(0, powerupDriver->getTexture(Spread));
				powerupNode->setVisible(true);
				powerupNode->setPosition(powerUpPosition);
			}
			powerup->SetPowerup(powerupNode);
		}
	}
	//To check the chance of power up drop
	cout << powerUpChance;
}
