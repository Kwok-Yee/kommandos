#include <irrlicht.h>
#include "Player.h"
#include "Game.h"
#include "InputReceiver.h"
#include "Gameoverstate.h"
#include "Collision.h"
#include "Gun.h"
#include "Score.h"
#include "BulletPool.h"
#include "Bullet.h"
#include "iostream"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace std;

const s32 x1Bar = 10, y1Bar = 10, x2Bar = 10, y2Bar = 25; //healthbar size
#define MAXHEALTH 100 //bar size
// This is the movement speed in units per second.
#define MOVEMENT_SPEED 50.f
// Correction for rotating the Y Axis on the player object
#define Y_AXIS_CORRECTION -90.f

IrrlichtDevice* playerIDevice;
IVideoDriver* playerDriver;
ISceneManager* playerSmgr;
GameOverState gameOverState;
Collision playerCol;

//int vulnerable = 0;
Gun* gun;
Score playerScores;
Game* game;

ISceneNode* playerObject;
IMeshSceneNode* gunNode;
ISceneNode* bullet;
f32 health;
u32 time;
vector3df currentPosition;

BulletPool* pool;
Bullet* one;

// FRAMEDELTATIME
u32 now;
f32 frameDeltaTime;
bool hasShot = false;

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
	{
		playerObject->setPosition(vector3df(0, 0, 30));
	}
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
	}
	pool = pool->GetInstance();
	one = pool->GetResource();
	one->SetBullet(playerSmgr->addSphereSceneNode());
	if (one->GetBullet())
	{
		one->GetBullet()->setScale(vector3df(0.125f, 0.125f, 0.125f));
		one->GetBullet()->setPosition(vector3df(playerObject->getPosition()));
		one->GetBullet()->setVisible(false);
	}
}

void Player::Move(InputReceiver inputReceiver)
{
	// Work out a frame delta time.
	now = playerIDevice->getTimer()->getTime();
	frameDeltaTime = (f32)(now - time) / 1000.f; // Time in seconds
	time = now;

	vector3df newPosition = playerObject->getPosition();

	if (playerCol.CollidesWithStaticObjects(playerObject))
		currentPosition = playerObject->getPosition();

	if (inputReceiver.GetIsKeyDown(irr::KEY_KEY_W))
	{
		newPosition.X += frameDeltaTime * MOVEMENT_SPEED;
	}

	else if (inputReceiver.GetIsKeyDown(irr::KEY_KEY_S))
	{
		newPosition.X -= frameDeltaTime * MOVEMENT_SPEED;
	}

	if (inputReceiver.GetIsKeyDown(irr::KEY_KEY_A))
	{
		newPosition.Z += frameDeltaTime * MOVEMENT_SPEED;
	}
	else if (inputReceiver.GetIsKeyDown(irr::KEY_KEY_D))
	{
		newPosition.Z -= frameDeltaTime * MOVEMENT_SPEED;
	}

	playerObject->setPosition(newPosition);
	if (playerCol.CollidesWithStaticObjects(playerObject))
		playerObject->setPosition(currentPosition);
	
	// Calculate the angle using atan2 using the mouse position and the player object
	float angle = atan2(gun->GetMousePosition().Z - playerObject->getPosition().Z,
		gun->GetMousePosition().X - playerObject->getPosition().X);
	// Calculate the inverted angle
	angle *= -(RADTODEG);
	// Set the angle value to be between 0 and 360
	if (angle < 0)
	{
		angle = 360 - (-angle);
	}
	// Rotate player towards mouse position using the Y Axis correction and the calculated angle
	playerObject->setRotation(vector3df(0, Y_AXIS_CORRECTION + angle, 0));

	if (vulnerable > 0) { vulnerable -= frameDeltaTime; }
}

void Player::Shoot(InputReceiver inputReceiver, EnemySpawner* enemies)
{
	gun->LaserLine(inputReceiver.GetMousePosition(), playerDriver, playerSmgr->getActiveCamera());
	if (inputReceiver.GetIsLeftMouseButtonPressed()) 
	{
		//gun->Shoot(one->GetBullet());
		hasShot = true;
		one->GetBullet()->setVisible(true);
	}
	if (inputReceiver.GetIsKeyDown(KEY_F5)) playerIDevice->closeDevice();
	if (inputReceiver.GetIsKeyDown(KEY_KEY_X))
	{
		hasShot = false;
		pool->ReturnResource(one);
	}
	if (hasShot) 
	{
		one->UpdateBullet(gun->GetMousePosition(), playerObject->getPosition(), frameDeltaTime, 30.f);
		for (int i = 0; i < enemies->getEnemies().size(); i++) 
		{
			if (playerCol.SceneNodeWithSceneNode(enemies->getEnemies()[i], one->GetBullet())) 
			{
				enemies->enemyHealthValues[i] = enemies->getEnemyBehaviour()->TakeDamage(10, enemies->enemyHealthValues[i]);
				playerScores.DisplayScore(10);
			}
		}
		//gun->hasShot = false;
	}
}

void Player::TakeDamage(f32 damage, f32 frameDeltaTime)
{
	game = game->GetInstance();
	if (health > 0 && vulnerable <= 0) {
		vulnerable = 800;
		health -= damage;

		if (health <= 0)
		{
			gameOverState.ShowGameOver(playerIDevice);
			game->SetIsGameOver(true);
		}
	}

}
void Player::DrawHealthBar()
{
	const s32 barSize = MAXHEALTH;
	//draws multiple bars to make i look nice
	playerDriver->draw2DRectangle(SColor(255, 100, 100, 100), rect<s32>(x1Bar, y1Bar, (barSize * 5) + x2Bar, y2Bar));
	playerDriver->draw2DRectangle(SColor(255, 125, 125, 125), rect<s32>(x1Bar + 1, y1Bar + 1, barSize * 5 + x2Bar - 1, y2Bar - 1));
	playerDriver->draw2DRectangle(SColor(255, 150, 150, 150), rect<s32>(x1Bar + 3, y1Bar + 3, barSize * 5 + x2Bar - 3, y2Bar - 3));
	playerDriver->draw2DRectangle(rect<s32>(x1Bar + 3, y1Bar + 3, health * 5 + x2Bar - 3, y2Bar - 3),
		SColor(255, 255 - health * 2.55, health*2.55, 0),
		SColor(255, 255 - health * 2.55, health*2.55, 0),
		SColor(255, 255 - health * 2.55, health*2.55 - 150, 0),
		SColor(255, 255 - health * 2.55, health*2.55 - 150, 0));

}

ISceneNode* Player::getPlayerObject() {
	return playerObject;
}
