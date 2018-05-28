#pragma once
#include <irrlicht.h>

class Gun
{
public:
	Gun(irr::scene::ISceneNode* gun, irr::IrrlichtDevice* device);
	// bullet properties
	void Shoot(irr::scene::ISceneNode* bullet);

	void BulletAnimation(float animSpeed, irr::scene::ISceneNode * bullet);

	// Raycast from screen with a start to end position and the laserline.
	void LaserLine(irr::core::vector3df endPosition, irr::video::IVideoDriver* driver, irr::scene::ICameraSceneNode* camera);
	void RayFromScreen(irr::core::vector3df &endPosition, irr::scene::ICameraSceneNode * camera);

	// gets the ray start and it's vector to the end position which is the mousePosition
	bool OnLineIntersect(irr::core::plane3df &plane, irr::core::line3df &ray);

	//animation & bullet checks
	bool CheckAnimEnd(irr::scene::ISceneNode* bullet);
	bool BulletShootAnim(irr::scene::ISceneNode * bullet);
	bool hasShot;

	irr::core::vector3df GetMousePosition();
	
private:
	irr::scene::ISceneNode* gunNode;
	irr::core::vector3df mousePosition;
};

