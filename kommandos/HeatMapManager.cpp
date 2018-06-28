#include <irrlicht.h>
#include "HeatMapManager.h"
#include <iostream>
#include "Game.h"

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

HeatMapManager::HeatMapManager()
{
	zone1Weight = 0;
	zone2Weight = 0;
	zone3Weight = 0;
	zone4Weight = 0;
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
			cout << "zone 1 weight " << zone1Weight << endl;
		}
		else 
		{
			CreateCountdown(Zone1);
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
		seconds -= 0.016;

		stringw newText("There will be gas in ");
		newText += activeZone;
		newText += " in ";
		newText += seconds;
		newText += " seconds";
		countdownText->setText(newText.c_str());
		if (seconds < 0)
		{
			countdown = false;
			CreatePoisonCloud(activeZone);
			ui->clear();
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
	//This is used to calculate frame delta time
	seconds = MAX_SECONDS;
}

void HeatMapManager::CreatePoisonCloud(Zone zone)
{
	const path textPath = "../media/Textures/PoisonCloud.jpg";
	vector3df cloudPosition;
	cloudPosition.Y = -0.5;
	f32 size = level->getTransformedBoundingBox().getExtent().X / 18;
	switch (zone)
	{
	case 1:
		cloudPosition.X = -size;
		cloudPosition.Z = -size;
	case 2:
		cloudPosition.X = -size;
		cloudPosition.Z = size;
	case 3:
		cloudPosition.X = size;
		cloudPosition.Z = -size;
	case 4:
		cloudPosition.X = size;
		cloudPosition.Z = size;
	default:
		break;
	}
	cloudPosition.X *= 5;
	cloudPosition.Z *= 5;
	cout << "Creating Cloud at: " << cloudPosition.X << "," << cloudPosition.Z << " with size " << size << endl;
	poisonCloud = hsmgr->addCubeSceneNode();
	if (poisonCloud) {
		poisonCloud->setPosition(cloudPosition);
		poisonCloud->setScale(vector3df(size, 1.0, size));
		ITexture* texture = hGame->device->getVideoDriver()->getTexture(textPath);
		poisonCloud->setMaterialType(irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL);
		poisonCloud->setMaterialTexture(0, texture);
		//making the cloud a bit transparent
		for (int i = 0; i < poisonCloud->getMaterialCount(); i++)
		{
			SMaterial mat = poisonCloud->getMaterial(i);
			mat.BackfaceCulling = false;
			mat.Wireframe = false;
			mat.Lighting = false;
			mat.MaterialType = irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF;
			mat.DiffuseColor.setAlpha(20);

		}
	}
}

ISceneNode* HeatMapManager::GetPoisonCloud() {
	return poisonCloud;
}

