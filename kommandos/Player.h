#pragma once
#include <irrlicht.h>
#include "InputReceiver.h"
class Player
{
public:
	irr::core::vector3df currentPosition;
	Player(irr::IrrlichtDevice* device);
	void TakeDamage(irr::f32 damage);
	void DrawHealthBar();
	void Move(irr::scene::ISceneNode* playerNode, InputReceiver inputReceiver);
};

