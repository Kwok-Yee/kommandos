#include <irrlicht.h>
#include "HeatMapManager.h"
#include <iostream>
#include "Game.h"
#include "SoundManager.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace gui;
using namespace io;
using namespace std;

Game* hGame = hGame->GetInstance();
ISceneManager* hsmgr = hGame->device->getSceneManager();
IGUIEnvironment* ui = hGame->device->getGUIEnvironment();
SoundManager* hSoundManager;

HeatMapManager::HeatMapManager()
{
	zone1Weight = 0;
	zone2Weight = 0;
	zone3Weight = 0;
	zone4Weight = 0;
	hSoundManager = hSoundManager->GetInstance();
}

// Set instance to 0 (NULL)
HeatMapManager* HeatMapManager::instance = 0;

HeatMapManager* HeatMapManager::GetInstance()
{
	if (!instance)
	{
		instance = new HeatMapManager();
	}
	return instance;
}

HeatMapManager::Zone HeatMapManager::CheckZoneFromPosition(irr::core::vector3df pos)
{
	if (pos.X > 0 && pos.Z > 0)
	{
		return Zone1;
	}
	else if (pos.X > 0 && pos.Z < 0)
	{
		return Zone2;
	}
	else if (pos.X < 0 && pos.Z > 0)
	{
		return Zone3;
	}
	else if (pos.X < 0 && pos.Z < 0)
	{
		return Zone4;
	}
}

void HeatMapManager::AddWeight(Zone zone, float weight)
{
	if (isZoneActive) {
		return;
	}
	switch (zone)
	{
	case Zone1:
		if (zone1Weight < MAX_WEIGHT)
		{
			zone1Weight += weight;
		}
		else
		{
			CreateCountdown(Zone1);
			zone1Weight = 0;
		}
		break;
	case Zone2:
		if (zone2Weight < MAX_WEIGHT)
		{
			zone2Weight += weight;
		}
		else
		{
			CreateCountdown(Zone2);
			zone2Weight = 0;
		}
		break;
	case Zone3:
		if (zone3Weight < MAX_WEIGHT)
		{
			zone3Weight += weight;
		}
		else
		{
			CreateCountdown(Zone3);
			zone3Weight = 0;
		}
		break;
	case Zone4:
		if (zone4Weight < MAX_WEIGHT)
		{
			zone4Weight += weight;
		}
		else
		{
			CreateCountdown(Zone4);
			zone4Weight = 0;
		}
		break;
	default:
		break;
	}
}

int HeatMapManager::GetWeight(Zone zone)
{
	switch (zone)
	{
	case Zone1:
		return zone1Weight;
		break;
	case Zone2:
		return zone2Weight;
		break;
	case Zone3:
		return zone3Weight;
		break;
	case Zone4:
		return zone4Weight;
		break;
	default:
		break;
	}
}

void HeatMapManager::Update()
{
	if (countdown)
	{
		seconds -= 0.016; //we use vsync, so it's always 60fps, 0.016 is the framedeltatime
		stringw newText("There will be gas in Zone ");
		newText += activeZone;
		newText += " in ";
		newText += seconds;
		newText += " seconds";
		countdownText->setText(newText.c_str());

		isDangerZoneActive = true;
		if (seconds < 0)
		{
			countdown = false;
			CreatePoisonCloud(activeZone);
			countdownText->remove();
		}
	}
	if (isDangerZoneActive && dangerZoneSpawned == false)
	{
		CreateDangerZone(activeZone);
		dangerZoneSpawned = true;
		//isDangerZoneActive = false;
	}
	if (isPoisonCloudActive)
	{
		if (isDangerZoneActive)
		{
			isDangerZoneActive = false;
			dangerZoneSpawned == false;
			//dangerZone->remove();
			hsmgr->addToDeletionQueue(GetDangerZone());
		}
		//hsmgr->addToDeletionQueue(GetDangerZone());  <--- HERE's THE ERROR 
		seconds -= 0.016;
		if (seconds < 0)
		{
			hGame->objectPlacementGen.GenerateNewObjects(activeZone);
			hsmgr->addToDeletionQueue(GetPoisonCloud());
			//poisonCloud->setVisible(false);
			isZoneActive = false;
			isPoisonCloudActive = false;
			activeZone = Zone0;
		}
	}
}

void HeatMapManager::CreateCountdown(Zone zone)
{
	isZoneActive = true;
	countdown = true;
	activeZone = zone;
	stringw text("There will be gas in Zone ");
	text += zone;
	text += " in 5 seconds";
	countdownText = ui->addStaticText(text.c_str(), rect<int>(350, 80, 450, 120), true, true, 0, -1, true);
	countdownText->setMinSize(dimension2du(100, 40));
	countdownText->setMaxSize(dimension2du(100, 40));

	seconds = MAX_COUNTDOWN_SECONDS;
}
void HeatMapManager::CreateDangerZone(Zone zone)
{
	const path texturePathDangerZone = "../media/Textures/danger_texture.png";
	vector3df dangerZonePosition;
	dangerZonePosition.Y = -1.5;
	f32 size = level->getTransformedBoundingBox().getExtent().X / 18;
	switch (zone)
	{
	case Zone1:
		dangerZonePosition.X = size;
		dangerZonePosition.Z = size;
		break;
	case Zone2:
		dangerZonePosition.X = size;
		dangerZonePosition.Z = -size;
		break;
	case Zone3:
		dangerZonePosition.X = -size;
		dangerZonePosition.Z = size;
		break;
	case Zone4:
		dangerZonePosition.X = -size;
		dangerZonePosition.Z = -size;
		break;
	default:
		break;
	}
	dangerZonePosition.X *= 5;
	dangerZonePosition.Z *= 5;
	dangerZone = hsmgr->addCubeSceneNode();
	if (dangerZone) {
		dangerZone->setPosition(dangerZonePosition);
		dangerZone->setScale(vector3df(size, 4.0, size));

		dangerZone->setMaterialFlag(video::EMF_LIGHTING, false);
		dangerZone->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
		dangerZone->setMaterialTexture(0, hGame->device->getVideoDriver()->getTexture(texturePathDangerZone));
		dangerZone->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
		printf("dangerzone is made");

	}
	cout << dangerZone;
	//isDangerZoneActive = true;
}

void HeatMapManager::CreatePoisonCloud(Zone zone)
{
	//const path texturePathDangerZone = "../media/Textures/danger_texture.png";
	const path texturePath = "../media/Textures/PoisonCloud.jpg";
	vector3df cloudPosition;
	cloudPosition.Y = -0.5;
	f32 size = level->getTransformedBoundingBox().getExtent().X / 18;
	switch (zone)
	{
	case Zone1:
		cloudPosition.X = size;
		cloudPosition.Z = size;
		break;
	case Zone2:
		cloudPosition.X = size;
		cloudPosition.Z = -size;
		break;
	case Zone3:
		cloudPosition.X = -size;
		cloudPosition.Z = size;
		break;
	case Zone4:
		cloudPosition.X = -size;
		cloudPosition.Z = -size;
		break;
	default:
		break;
	}
	cloudPosition.X *= 5;
	cloudPosition.Z *= 5;
	poisonCloud = hsmgr->addCubeSceneNode();
	if (poisonCloud) {
		poisonCloud->setPosition(cloudPosition);
		poisonCloud->setScale(vector3df(size, 4.0, size));

		poisonCloud->setMaterialFlag(video::EMF_LIGHTING, false);
		poisonCloud->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
		poisonCloud->setMaterialTexture(0, hGame->device->getVideoDriver()->getTexture(texturePath));
		poisonCloud->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
	}
	isPoisonCloudActive = true;
	seconds = MAX_POISONCLOUS_SECONDS;
	hSoundManager->PlaySound(GAS_SOUND, false);
}

ISceneNode* HeatMapManager::GetDangerZone() {
	return dangerZone;
}

ISceneNode* HeatMapManager::GetPoisonCloud() {
	return poisonCloud;
}

