#include <irrlicht.h>
#include "HeatMapManager.h"
#include <iostream>

using namespace std;

HeatMapManager::HeatMapManager()
{
	zone1Weight = 0;
	zone2Weight = 0;
	zone3Weight = 0;
	zone4Weight = 0;
}

// Set instance to 0 (NULL)
HeatMapManager* HeatMapManager::instance = 0;

HeatMapManager* HeatMapManager::GetInstance() {
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
	switch (zone)
	{
	case Zone1:
		zone1Weight += weight;
		break;
	case Zone2:
		zone2Weight += weight;
		break;
	case Zone3:
		zone3Weight += weight;
		break;
	case Zone4:
		zone4Weight += weight;
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

