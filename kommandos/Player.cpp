#include "Player.h"
#include <irrlicht.h>
#include "InputReceiver.h"

using namespace irr;

using namespace core;
using namespace std;

// This is the movement speed in units per second.
const f32 MOVEMENT_SPEED = 50.f;
u32 then;
IrrlichtDevice* iDevice;

Player::Player(IrrlichtDevice* device)
{
	health = 0;
	iDevice = device;

	// In order to do framerate independent movement, we have to know
	// how long it was since the last frame
	then = iDevice->getTimer()->getTime();
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
}
