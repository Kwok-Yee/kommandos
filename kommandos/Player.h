#pragma once
#include <irrlicht.h>
#include "InputReceiver.h"
class Player
{
public:
	Player(irr::IrrlichtDevice* device);
	void TakeDamage(irr::f32 damage);
	void DrawHealthBar();
	void Move(InputReceiver inputReceiver);
	irr::scene::ISceneNode* getPlayerObject();
};

