#include "Player.h"
#include <irrlicht.h>
#include <iostream>
#include "Game.h"
#include "InputReceiver.h"
#include "Gameoverstate.h"
#include "Collision.h"
#include "Gun.h"
#include "Score.h"

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
Collision _collision;

//int vulnerable = 0;
Gun* gun;
Score scores;
Game* game;

ISceneNode* playerObject;
IMeshSceneNode* gunNode;
ISceneNode* bullet;
f32 health;
u32 time;
vector3df currentPosition;

Player::Player(IrrlichtDevice* device)
{
	playerIDevice = device;
	playerDriver = playerIDevice->getVideoDriver();
	playerSmgr = playerIDevice->getSceneManager();
	Init();

	// In order to do framerate independent movement, we have to know
	// how long it was since the last frame
	time = playerIDevice->getTimer()->getTime();
}

void Player::Init() 
{
	health = MAXHEALTH;
	IMesh* playerMesh = playerSmgr->getMesh("../media/PlayerModel.3ds");
	playerObject = playerSmgr->addMeshSceneNode(playerMesh);
	if (playerObject)
		playerObject->setPosition(vector3df(0, 0, 30));
	currentPosition = playerObject->getPosition();

	IMesh* gunModel = playerSmgr->getMesh("../media/LowPoly_Irrlicht.3ds");
	gunNode = playerSmgr->addMeshSceneNode(gunModel);
	bullet = playerSmgr->addSphereSceneNode();
	if (gunNode)
	{
		gunNode->setPosition(vector3df(2, 5, -1));
		gunNode->setScale(vector3df(0.125f, 0.125f, 0.125f));
		gunNode->setMaterialFlag(EMF_LIGHTING, false);
		gunNode->setMaterialTexture(0, playerDriver->getTexture("../media/Gun_Color.png"));
		playerObject->addChild(gunNode);
		gun = new Gun(gunNode, playerIDevice);
	}
	if (bullet) {
		bullet->setScale(vector3df(0.125f, 0.125f, 0.125f));
		gunNode->setMaterialFlag(EMF_LIGHTING, false);
		bullet->setVisible(false);
		//gunNode->addChild(bullet);
	}
}


void Player::Move(InputReceiver inputReceiver)
{
	// Work out a frame delta time.
	const u32 now = playerIDevice->getTimer()->getTime();
	const f32 frameDeltaTime = (f32)(now - time) / 1000.f; // Time in seconds
	time = now;

	vector3df newPosition = playerObject->getPosition();

	if (_collision.CollidesWithStaticObjects(playerObject))
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

		playerObject->setMaterialFlag(video::EMF_LIGHTING, joystickData.IsButtonPressed(7));

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
			playerObject->setRotation(core::vector3df(0, atan2(vRot, hRot) * 180 / PI, 0));
		}
		else {
			playerObject->setRotation(core::vector3df(0, atan2(prevVRot, prevHRot) * 180 / PI, 0));
		}
	}

	playerObject->setPosition(newPosition);
	if (_collision.CollidesWithStaticObjects(playerObject))
		playerObject->setPosition(currentPosition);

	if (vulnerable > 0) { vulnerable -= frameDeltaTime; }
}

void Player::Shoot(InputReceiver inputReceiver, EnemySpawner* enemies)
{
	if (inputReceiver.isLeftMouseButtonDown) {
		gun->LaserLine(inputReceiver.position, playerDriver, playerSmgr->getActiveCamera());
		gun->Shoot(bullet);
	}
	if (gun->hasShot) {
		for (int i = 0; i < enemies->getEnemies().size(); i++) {
			if (_collision.SceneNodeWithSceneNode(enemies->getEnemies()[i], bullet)) {
				enemies->enemyHealthValues[i] = enemies->getEnemyBehaviour()->TakeDamage(10, enemies->enemyHealthValues[i]);
				scores.DisplayScore(10);
			}
		}
	}
}

void Player::TakeDamage(f32 damage, f32 frameDeltaTime)
{
	if (health > 0 && vulnerable <= 0 ) {
		vulnerable = 800;
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
