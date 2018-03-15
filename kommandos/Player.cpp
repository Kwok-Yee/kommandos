#include "Player.h"
#include <irrlicht.h>
#include "InputReceiver.h"

using namespace irr;
using namespace core;
using namespace std;

irr::f32 health;
const irr::s32 X1_BAR = 10, Y1_BAR = 10, X2_BAR = 10, Y2_BAR = 25; //healthbar size
const irr::s32 MAXHEALTH = 100; //bar size

// This is the movement speed in units per second.
const f32 MOVEMENT_SPEED = 50.f;

u32 then;
IrrlichtDevice* iDevice;
video::IVideoDriver* driver;

Player::Player(IrrlichtDevice* device)
{
	health = MAXHEALTH;
	iDevice = device;
	driver = device->getVideoDriver();
	// In order to do framerate independent movement, we have to know
	// how long it was since the last frame
	then = iDevice->getTimer()->getTime();
	DrawHealthBar();
}

vector3df Player::Move(vector3df oldPosition, InputReceiver inputReceiver)
{
	// Work out a frame delta time.
	const u32 now = iDevice->getTimer()->getTime();
	const f32 frameDeltaTime = (f32)(now - then) / 1000.f; // Time in seconds
	then = now;
	
	vector3df newposition;
	if (inputReceiver.IsKeyDown(irr::KEY_KEY_W))
		 oldPosition.X += MOVEMENT_SPEED * frameDeltaTime;
	else if (inputReceiver.IsKeyDown(irr::KEY_KEY_S))
		oldPosition.X -= MOVEMENT_SPEED * frameDeltaTime;

	if (inputReceiver.IsKeyDown(irr::KEY_KEY_A))
		oldPosition.Z += MOVEMENT_SPEED * frameDeltaTime;
	else if (inputReceiver.IsKeyDown(irr::KEY_KEY_D))
		oldPosition.Z -= MOVEMENT_SPEED * frameDeltaTime;
	return oldPosition;
}

void Player::TakeDamage(f32 damage)
{
	health -= damage;
	if (health <= 0) 
	{
		//Game over screen.
	}
	DrawHealthBar();
}
void Player::DrawHealthBar()
{
	driver->draw2DRectangle(video::SColor(255, 100, 100, 100), core::rect<s32>(X1_BAR, Y1_BAR, (MAXHEALTH * 5) + X2_BAR, Y2_BAR));

	driver->draw2DRectangle(video::SColor(255, 125, 125, 125), core::rect<s32>(X1_BAR + 1, Y1_BAR + 1, MAXHEALTH * 5 + X2_BAR - 1, Y2_BAR - 1));

	driver->draw2DRectangle(video::SColor(255, 150, 150, 150), core::rect<s32>(X1_BAR + 3, Y1_BAR + 3, MAXHEALTH * 5 + X2_BAR - 3, Y2_BAR - 3));

	driver->draw2DRectangle(core::rect<s32>(X1_BAR + 3, Y1_BAR + 3, health * 5 + X2_BAR - 3, Y2_BAR - 3),

		video::SColor(255, 255 - health * 2.55, health*2.55, 0),

		video::SColor(255, 255 - health * 2.55, health*2.55, 0),

		video::SColor(255, 255 - health * 2.55, health*2.55 - 150, 0),

		video::SColor(255, 255 - health * 2.55, health*2.55 - 150, 0));
}
