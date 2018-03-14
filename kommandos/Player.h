#pragma once
#include <irrlicht.h>
#include "InputReceiver.h"
class Player
{
public:
	Player(irr::IrrlichtDevice* device);
	void TakeDamage(irr::f32 damage);
	irr::core::vector3df Move(irr::core::vector3df oldPosition, InputReceiver inputReceiver);
	void DrawHealthBar();
private:
};

