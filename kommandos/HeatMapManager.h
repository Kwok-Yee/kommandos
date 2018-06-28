#pragma once
#include <irrlicht.h>

#define MAX_WEIGHT 100
#define MAX_SECONDS 5
class HeatMapManager
{
public:
	enum Zone
	{
		Zone0,//No Zone
		Zone1,//Left-Top
		Zone2,//Right-Top
		Zone3,//Left-Bot
		Zone4//Right-Bot
	};
	irr::scene::IMeshSceneNode* level;
	HeatMapManager(const HeatMapManager&) = delete;
	HeatMapManager& operator=(const HeatMapManager&) = delete;
	//Returns the Game instance
	static HeatMapManager* GetInstance();
	Zone activeZone;
	bool isPoisonCloudActive = false;
	void AddWeight(Zone zone, float weight);
	int GetWeight(Zone zone);
	Zone CheckZoneFromPosition(irr::core::vector3df pos);
	void Update();
	irr::scene::ISceneNode* GetPoisonCloud();
private:
	HeatMapManager();
	static HeatMapManager* instance;
	float zone1Weight, zone2Weight, zone3Weight, zone4Weight;
	bool isZoneActive = false, countdown = false;
	float seconds;
	irr::scene::ISceneNode* poisonCloud;
	irr::gui::IGUIStaticText* countdownText;
	void CreatePoisonCloud(Zone zone);
	void CreateCountdown(Zone zone);

};

