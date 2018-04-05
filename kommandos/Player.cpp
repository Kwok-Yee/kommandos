#include "Player.h"
#include <irrlicht.h>
#include "InputReceiver.h"
#include "Gameoverstate.h"
#include "Collision.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace std;

const s32 X1_BAR = 10, Y1_BAR = 10, X2_BAR = 10, Y2_BAR = 25; //healthbar size
const s32 MAXHEALTH = 100; //bar size
// This is the movement speed in units per second.
const f32 MOVEMENT_SPEED = 50.f;

IrrlichtDevice* playerIDevice;
IVideoDriver* playerDriver;
ISceneManager* playerSmgr;
GameOverState gameOverState;
Collision collision;

ISceneNode* playerObject;
f32 health;
u32 time;
vector3df currentPosition;

Player::Player(IrrlichtDevice* device)
{
	playerIDevice = device;
	playerDriver = playerIDevice->getVideoDriver();
	playerSmgr = playerIDevice->getSceneManager();

	health = MAXHEALTH;
	IMesh* playerMesh = playerSmgr->getMesh("../media/PlayerModel.3ds");
	playerObject = playerSmgr->addMeshSceneNode(playerMesh);
	if (playerObject)
		playerObject->setPosition(vector3df(0, 0, 30));
	currentPosition = playerObject->getPosition();
	// In order to do framerate independent movement, we have to know
	// how long it was since the last frame
	time = playerIDevice->getTimer()->getTime();
}

void Player::Move(InputReceiver inputReceiver)
{
	// Work out a frame delta time.
	const u32 now = playerIDevice->getTimer()->getTime();
	const f32 frameDeltaTime = (f32)(now - time) / 1000.f; // Time in seconds
	time = now;

	vector3df newPosition = playerObject->getPosition();

	if (collision.CollidesWithStaticObjects(playerObject))
		currentPosition = playerObject->getPosition();

	if (inputReceiver.IsKeyDown(irr::KEY_KEY_W))
	{
		newPosition.X += MOVEMENT_SPEED * frameDeltaTime;
		playerObject->setRotation(vector3df(0, -90, 0));
	}
	else if (inputReceiver.IsKeyDown(irr::KEY_KEY_S))
	{
		newPosition.X -= MOVEMENT_SPEED * frameDeltaTime;
		playerObject->setRotation(vector3df(0, 90, 0));
	}

	if (inputReceiver.IsKeyDown(irr::KEY_KEY_A))
	{
		newPosition.Z += MOVEMENT_SPEED * frameDeltaTime;
		playerObject->setRotation(vector3df(0, -180, 0));
	}
	else if (inputReceiver.IsKeyDown(irr::KEY_KEY_D))
	{
		newPosition.Z -= MOVEMENT_SPEED * frameDeltaTime;
		playerObject->setRotation(vector3df(0, 0, 0));
	}

	playerObject->setMaterialFlag(video::EMF_LIGHTING, inputReceiver.isLeftMouseButtonDown);

	playerObject->setPosition(newPosition);
	if (collision.CollidesWithStaticObjects(playerObject))
		playerObject->setPosition(currentPosition);
}

void Player::TakeDamage(f32 damage)
{
	if (health > 0) {
		health -= damage;

		if (health <= 0)
			gameOverState.ShowGameOver(playerIDevice);
	}
}
void Player::DrawHealthBar()
{
	playerDriver->draw2DRectangle(SColor(255, 100, 100, 100), rect<s32>(X1_BAR, Y1_BAR, (MAXHEALTH * 5) + X2_BAR, Y2_BAR));
	playerDriver->draw2DRectangle(SColor(255, 125, 125, 125), rect<s32>(X1_BAR + 1, Y1_BAR + 1, MAXHEALTH * 5 + X2_BAR - 1, Y2_BAR - 1));
	playerDriver->draw2DRectangle(SColor(255, 150, 150, 150), rect<s32>(X1_BAR + 3, Y1_BAR + 3, MAXHEALTH * 5 + X2_BAR - 3, Y2_BAR - 3));
	playerDriver->draw2DRectangle(rect<s32>(X1_BAR + 3, Y1_BAR + 3, health * 5 + X2_BAR - 3, Y2_BAR - 3),
		SColor(255, 255 - health * 2.55, health*2.55, 0),
		SColor(255, 255 - health * 2.55, health*2.55, 0),
		SColor(255, 255 - health * 2.55, health*2.55 - 150, 0),
		SColor(255, 255 - health * 2.55, health*2.55 - 150, 0));
}

ISceneNode* Player::getPlayerObject() {
	return playerObject;
}
