#include "Player.h"
#include <irrlicht.h>
#include <iostream>
#include "InputReceiver.h"
#include "Gameoverstate.h"
#include "Collision.h"

using namespace irr;
using namespace core;
using namespace std;

const irr::s32 X1_BAR = 10, Y1_BAR = 10, X2_BAR = 10, Y2_BAR = 25; //healthbar size
const irr::s32 MAXHEALTH = 100; //bar size
// This is the movement speed in units per second.
const f32 MOVEMENT_SPEED = 50.f;

irr::f32 health;
u32 then;
IrrlichtDevice* iDevice;
video::IVideoDriver* driver;
GameOverState gameOverState;
Collision collision;
int vulnerable = 0;

Player::Player(IrrlichtDevice* device)
{
	health = MAXHEALTH;
	iDevice = device;
	driver = device->getVideoDriver();
	// In order to do framerate independent movement, we have to know
	// how long it was since the last frame
	then = iDevice->getTimer()->getTime();
}

void Player::Move(irr::scene::ISceneNode* playerNode, InputReceiver inputReceiver)
{
	// Work out a frame delta time.
	const u32 now = iDevice->getTimer()->getTime();
	const f32 frameDeltaTime = (f32)(now - then) / 1000.f; // Time in seconds
	then = now;

	vector3df newPosition = playerNode->getPosition();

	if (collision.CollidesWithStaticObjects(playerNode))
		currentPosition = playerNode->getPosition();

	if (inputReceiver.IsKeyDown(irr::KEY_KEY_W))
	{
		newPosition.X += MOVEMENT_SPEED * frameDeltaTime;
		playerNode->setRotation(vector3df(0, -90, 0));
	}
	else if (inputReceiver.IsKeyDown(irr::KEY_KEY_S))
	{
		newPosition.X -= MOVEMENT_SPEED * frameDeltaTime;
		playerNode->setRotation(vector3df(0, 90, 0));
	}

	if (inputReceiver.IsKeyDown(irr::KEY_KEY_A))
	{
		newPosition.Z += MOVEMENT_SPEED * frameDeltaTime;
		playerNode->setRotation(vector3df(0, -180, 0));
	}
	else if (inputReceiver.IsKeyDown(irr::KEY_KEY_D))
	{
		newPosition.Z -= MOVEMENT_SPEED * frameDeltaTime;
		playerNode->setRotation(vector3df(0, 0, 0));
	}

	playerNode->setMaterialFlag(video::EMF_LIGHTING, inputReceiver.isLeftMouseButtonDown);

	playerNode->setPosition(newPosition);
	if (collision.CollidesWithStaticObjects(playerNode))
		playerNode->setPosition(currentPosition);

	if (vulnerable > 0) { vulnerable -= frameDeltaTime; }
}

void Player::TakeDamage(f32 damage, f32 frameDeltaTime)
{
	if (health > 0 && vulnerable <= 0 ) {
		vulnerable = 500;
		health -= damage;

		if (health <= 0)
			gameOverState.ShowGameOver(iDevice);
	}
	
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
