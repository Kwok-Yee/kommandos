#pragma once
#include <irrlicht.h>
#include "InputReceiver.h"
class Player
{
public:
	irr::f32 health;
	Player(irr::IrrlichtDevice* device);
	void TakeDamage(irr::f32 damage);
	irr::core::vector3df Move(irr::core::vector3df oldPosition, InputReceiver inputReceiver);
	irr::core::vector3df playerRotate(irr::core::vector3df playerRotation, InputReceiver inputReceiver);
	void DrawHealthBar();
private:
};

