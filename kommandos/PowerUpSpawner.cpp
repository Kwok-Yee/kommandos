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

	PowerUpSpawn(vector3df(280,0,280));

}

void PowerUpSpawner::PowerUpSpawn(vector3df position)
{
	powerup = pool->GetResource();
	powerup->SetPowerupType(1);
	//powerup->GetNode()->setPosition(position);
	powCol.AddPowerupToList(powerup);

}
