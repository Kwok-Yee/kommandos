#include "Player.h"
#include <irrlicht.h>
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

	if (inputReceiver.GetJoystickInfo().size() > 0)
	{
		f32 hMove = 0.f;
		f32 vMove = 0.f;
		f32 moveSpeed = 50.f;

		f32 hRot = 0.f;
		f32 vRot = 0.f;
		f32 prevHRot = 0.f;
		f32 prevVRot = 0.f;

		// Store a local reference from joystickState
		const SEvent::SJoystickEvent & joystickData = inputReceiver.joystickState;

		// Base dead zone for the controller axis, user should be able to change this in-game
		const f32 DEAD_ZONE = 0.05f;

		hMove = (f32)joystickData.Axis[SEvent::SJoystickEvent::AXIS_Y] / -32767.f;
		if (fabs(hMove) < DEAD_ZONE)
			hMove = 0.f;
		vMove = (f32)joystickData.Axis[SEvent::SJoystickEvent::AXIS_X] / -32767.f;
		if (fabs(vMove) < DEAD_ZONE)
			vMove = 0.f;

		if (!core::equals(vMove, 0.f) || !core::equals(hMove, 0.f))
		{
			newPosition.X += moveSpeed * frameDeltaTime * hMove;
			newPosition.Z += moveSpeed * frameDeltaTime * vMove;
		}

		playerNode->setMaterialFlag(video::EMF_LIGHTING, joystickData.IsButtonPressed(7));

		vRot = (f32)joystickData.Axis[SEvent::SJoystickEvent::AXIS_V] / 32767.f;
		if (fabs(vRot) < DEAD_ZONE + 0.05f) {
			prevVRot = vRot;
			vRot = 0.f;
		}
		hRot = (f32)joystickData.Axis[SEvent::SJoystickEvent::AXIS_Z] / 32767.f;
		if (fabs(hRot) < DEAD_ZONE + 0.05f) {
			prevHRot = hRot;
			hRot = 0.f;
		}

		if (fabs(hRot) != 0 || fabs(vRot) != 0) {
			playerNode->setRotation(core::vector3df(0, atan2(vRot, hRot) * 180 / PI, 0));
		}
		else {
			playerNode->setRotation(core::vector3df(0, atan2(prevVRot, prevHRot) * 180 / PI, 0));
		}

		playerNode->setPosition(newPosition);
		if (collision.CollidesWithStaticObjects(playerNode))
			playerNode->setPosition(currentPosition);
	}
}

void Player::TakeDamage(f32 damage)
{
	if (health > 0) {
		health -= damage;

		if (health <= 0) {
			gameOverState.ShowGameOver(iDevice);
		}
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
