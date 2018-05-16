#pragma once
#include <irrlicht.h>
#include "InputReceiver.h"
#include "EnemySpawner.h"

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
	//vulnerable timer so you can't get hit multiple times in one second
	int vulnerable = 0;

private:
	//Initializes all variables and objects
	void Init();
};

