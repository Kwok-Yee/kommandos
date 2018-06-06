#pragma once
#include <irrlicht.h>
#include "InputReceiver.h"
#include "EnemySpawner.h"

using namespace irr;
using namespace core;
using namespace scene;

class Player
{
public:
	Player(irr::IrrlichtDevice* device);
	void TakeDamage(irr::f32 damage, irr::f32 frameDeltaTime);
	//Draws the healtbar in the top-left corner
	void DrawHealthBar();
	//Moves player with WASD keys
	void Move(InputReceiver inputReceiver);
	//Shoots a bullet with right mouse clcik
	void Shoot(InputReceiver inputReceiver, EnemySpawner* enemies);
	//Retruns the player object
	irr::scene::ISceneNode* getPlayerObject();
	// Raycast from screen with a start to end position and the laserline.
	void Raycast(irr::core::vector3df endPosition, irr::scene::ICameraSceneNode* camera);
	// gets the ray start and it's vector to the end position which is the mousePosition
	bool OnLineIntersect(irr::core::plane3df &plane, irr::core::line3df &ray);
	// Get player vulnerability
	irr::s32 getVulnerableTimer();
	// Get mouse position in 3d world space
	irr::core::vector3df GetMousePosition();

private:
	//Initializes all variables and objects
	void Init();
};

