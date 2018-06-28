#include <irrlicht.h>
#include <irrKlang.h>
#include "Player.h"
#include "Game.h"
#include "InputReceiver.h"
#include "Gameoverstate.h"
#include "Collision.h"
#include "Score.h"
#include "BulletPool.h"
#include "Bullet.h"
#include "SoundManager.h"
#include "HeatMapManager.h"
#include "iostream"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace std;
using namespace irrklang;

// Healthbar
#define X1BAR 10
#define X2BAR 10
#define Y1BAR 10
#define Y2BAR 25
#define MAX_HEALTH 100

// This is the movement speed in units per second.
#define MOVEMENT_SPEED 50.f
// Correction for rotating the Y Axis on the player object
#define Y_AXIS_CORRECTION -90.f

#define PLAYER_MODEL "../media/Models/player/PlayerModel.3ds"
#define GUN_MODEL "../media/Models/weapons/LowPoly_Irrlicht.3ds"
#define GUN_COLOR "../media/Textures/Gun_Color.png"
#define GUN_SHOT_SOUND "../media/Sounds/gunshot02.mp3"
#define TAKE_DAMAGE_SOUND "../media/Sounds/takedamage.mp3"

#define VULNERABLE_BASE_TIMER 75
#define BULLET_BASE_TIMER 30

IrrlichtDevice* playerIDevice;
IVideoDriver* playerDriver;
ISceneManager* playerSmgr;
SoundManager* soundManager;
GameOverState gameOverState;
Collision playerCol;
HeatMapManager* heatMapManager = heatMapManager->GetInstance();

Score playerScores;
Game* game;

ISceneNode* playerObject;
ISceneNode* gunNode;
f32 health;
u32 time;
vector3df currentPosition;

vector3df mousePosition;
vector3df toMousePos;

s32 vulnerableTimer = 0;

BulletPool* pool;
core::array<Bullet*> activeBullets;
s32 bulletTimer = 0;

// FRAMEDELTATIME
f32 frameDeltaTime;
bool hasShot = false;

Player::Player(IrrlichtDevice* device)
{
	soundManager = soundManager->GetInstance();
	playerIDevice = device;
	playerDriver = playerIDevice->getVideoDriver();
	playerSmgr = playerIDevice->getSceneManager();
	game = game->GetInstance();
	Init();

	// In order to do framerate independent movement, we have to know
	// how long it was since the last frame
	time = playerIDevice->getTimer()->getTime();
}

void Player::Init()
{
	health = MAX_HEALTH;
	IMesh* playerMesh = playerSmgr->getMesh(PLAYER_MODEL);
	playerObject = playerSmgr->addMeshSceneNode(playerMesh);
	if (playerObject)
	{
		playerObject->setPosition(vector3df(266, 0, 266));
	}
	currentPosition = playerObject->getPosition();

	playerCol.AddDynamicToList(playerObject);

	IMesh* gunModel = playerSmgr->getMesh(GUN_MODEL);
	gunNode = playerSmgr->addMeshSceneNode(gunModel);
	if (gunNode)
	{
		gunNode->setPosition(vector3df(2, 5, -1));
		gunNode->setScale(vector3df(0.125f, 0.125f, 0.125f));
		gunNode->setMaterialFlag(EMF_LIGHTING, true);
		gunNode->setMaterialTexture(0, playerDriver->getTexture(GUN_COLOR));
		playerObject->addChild(gunNode);
	}

	// Get the instance of BulletPool
	pool = pool->GetInstance();
	// Set the timer to the bullet base time
	bulletTimer = BULLET_BASE_TIMER;
}

void Player::Move(InputReceiver inputReceiver)
{
	// Work out a frame delta time.
	u32 now = playerIDevice->getTimer()->getTime();
	frameDeltaTime = (f32)(now - time) / 1000.f;
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

	heatMapManager->AddWeight(heatMapManager->CheckZoneFromPosition(newPosition), frameDeltaTime*5);
	if (heatMapManager->CheckZoneFromPosition(newPosition) == heatMapManager->activeZone && heatMapManager->isPoisonCloudActive) {
		TakeDamage(1, frameDeltaTime);
	}
	heatMapManager->Update();
	// Calculate the angle using atan2 using the mouse position and the player object
	float angle = atan2(mousePosition.Z - playerObject->getPosition().Z,
		mousePosition.X - playerObject->getPosition().X);
	// Calculate the inverted angle
	angle *= -(RADTODEG);
	// Set the angle value to be between 0 and 360
	if (angle < 0)
	{
		angle = 360 - (-angle);
	}
	// Rotate player towards mouse position using the Y Axis correction and the calculated angle
	playerObject->setRotation(vector3df(0, Y_AXIS_CORRECTION + angle, 0));

	if (vulnerableTimer > 0)
	{
		vulnerableTimer -= frameDeltaTime;
	}
}

void Player::Shoot(InputReceiver inputReceiver, EnemySpawner* enemies)
{
	Raycast(inputReceiver.GetMousePosition(), playerSmgr->getActiveCamera());
	if (bulletTimer > 0)
	{
		bulletTimer -= frameDeltaTime;
	}
	if (inputReceiver.GetIsLeftMouseButtonPressed() && bulletTimer <= 0)
	{
		soundManager->PlaySound(GUN_SHOT_SOUND, false);
		Bullet* bullet = pool->GetResource();
		activeBullets.push_back(bullet);
		bullet->SetBullet(playerSmgr->addSphereSceneNode());
		if (bullet->GetBullet())
		{
			bullet->GetBullet()->setVisible(false);
			bullet->GetBullet()->setScale(vector3df(0.125f, 0.125f, 0.125f));
			bullet->GetBullet()->setPosition(vector3df(playerObject->getPosition()));
		}
		hasShot = true;
		bullet->GetBullet()->setVisible(true);
		bulletTimer = BULLET_BASE_TIMER;
	}
	if (hasShot)
	{
		if (!activeBullets.empty())
		{
			for (int i = 0; i < activeBullets.size(); i++)
			{
				activeBullets[i]->UpdateBullet(mousePosition, playerObject->getPosition(), frameDeltaTime);
				for (int j = 0; j < enemies->getEnemies().size(); j++)
				{
					if (playerCol.SceneNodeWithSceneNode(enemies->getEnemies()[j], activeBullets[i]->GetBullet()))
					{
						enemies->enemyHealthValues[j] = enemies->getEnemyBehaviour()->TakeDamage(activeBullets[i]->GetDamage(), enemies->enemyHealthValues[j]);
						playerScores.DisplayScore(10);
						pool->ReturnResource(activeBullets[i]);
						activeBullets.erase(i);
						// return to correct the index
						return;
					}
				}
				if (activeBullets[i] && playerCol.CollidesWithStaticObjects(activeBullets[i]->GetBullet()))
				{
					pool->ReturnResource(activeBullets[i]);
					activeBullets.erase(i);
				}
				if (activeBullets.empty())
				{
					hasShot = false;
				}
			}
		}
	}
}

void Player::TakeDamage(f32 damage, f32 frameDeltaTime)
{
	if (health > 0 && vulnerableTimer <= 0)
	{
		soundManager->PlaySound(TAKE_DAMAGE_SOUND, false);
		vulnerableTimer = VULNERABLE_BASE_TIMER;
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
	if (game->GetIsGameOver() != true)
	{
		const s32 barSize = MAX_HEALTH;
		//draws multiple bars to make i look nice
		playerDriver->draw2DRectangle(SColor(255, 100, 100, 100), rect<s32>(X1BAR, Y1BAR, (barSize * 5) + X2BAR, Y2BAR));
		playerDriver->draw2DRectangle(SColor(255, 125, 125, 125), rect<s32>(X1BAR + 1, Y1BAR + 1, barSize * 5 + X2BAR - 1, Y2BAR - 1));
		playerDriver->draw2DRectangle(SColor(255, 150, 150, 150), rect<s32>(X1BAR + 3, Y1BAR + 3, barSize * 5 + X2BAR - 3, Y2BAR - 3));
		playerDriver->draw2DRectangle(rect<s32>(X1BAR + 3, Y1BAR + 3, health * 5 + X2BAR - 3, Y2BAR - 3),
			SColor(255, 255 - health * 2.55, health*2.55, 0),
			SColor(255, 255 - health * 2.55, health*2.55, 0),
			SColor(255, 255 - health * 2.55, health*2.55 - 150, 0),
			SColor(255, 255 - health * 2.55, health*2.55 - 150, 0));
	}
}

ISceneNode* Player::getPlayerObject()
{
	return playerObject;
}

void Player::Raycast(vector3df endPosition, ICameraSceneNode* camera)
{
	vector3df planeNormal = vector3df(0, -1, 0);
	vector2di rayScreenCoordination = vector2di(endPosition.X, endPosition.Z);
	// Create a ray through the screen coordinates.
	line3df ray = playerSmgr->getSceneCollisionManager()->getRayFromScreenCoordinates(rayScreenCoordination, camera);

	plane3df plane(playerObject->getPosition(), planeNormal);
	if (OnLineIntersect(plane, ray))
	{
		toMousePos = vector3df(mousePosition - gunNode->getPosition());
	}
}

// intersect the ray with a plane around the node facing towards the mouse.
bool Player::OnLineIntersect(irr::core::plane3df &plane, irr::core::line3df &ray)
{
	return plane.getIntersectionWithLine(ray.start, ray.getVector(), mousePosition);
}

s32 Player::getVulnerableTimer()
{
	return vulnerableTimer;
}

vector3df Player::GetMousePosition()
{
	return mousePosition;
}