#pragma once
#include <irrlicht.h>
#include "InputReceiver.h"
#include "EnemySpawner.h"

class Player
{
public:
	Player(irr::IrrlichtDevice* device);
	void TakeDamage(irr::f32 damage, irr::f32 frameDeltaTime);
	void DrawHealthBar();
	void Move(InputReceiver inputReceiver);
	void Shoot(InputReceiver inputReceiver, EnemySpawner* enemies);
	irr::scene::ISceneNode* getPlayerObject();
private:
	void Init();
};

