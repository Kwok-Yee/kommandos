#pragma once
#include <irrlicht.h>
class HeatMapManager
{
public:
	enum Zone
	{
		Zone1,//Left-Top
		Zone2,//Right-Top
		Zone3,//Left-Bot
		Zone4//Right-Bot
	};
	HeatMapManager(const HeatMapManager&) = delete;
	HeatMapManager& operator=(const HeatMapManager&) = delete;
	//Returns the Game instance
	static HeatMapManager* GetInstance();
	void AddWeight(Zone zone, float weight);
	int GetWeight(Zone zone);
	Zone CheckZoneFromPosition(irr::core::vector3df pos);
private:
	HeatMapManager();
	static HeatMapManager* instance;
	float zone1Weight, zone2Weight, zone3Weight, zone4Weight;

};

