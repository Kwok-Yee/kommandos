#pragma once
#include <irrlicht.h>

class Gun
{
public:
	Gun(irr::scene::ISceneNode* gun, irr::IrrlichtDevice* device);
	void Shoot(irr::scene::ISceneNode* bullet);
	void LaserLine(irr::core::vector3df endPosition, irr::video::IVideoDriver* driver, irr::scene::ICameraSceneNode* camera);
	bool CheckAnimEnd(irr::scene::ISceneNode* bullet);
	bool hasShot;
	
private:
	irr::scene::ISceneNode* gunNode;
	
};

