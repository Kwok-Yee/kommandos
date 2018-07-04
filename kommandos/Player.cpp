///-------------------------------------------------------------------------------------------------
// file:	Player.cpp
//
// summary:	Implements the player class
///-------------------------------------------------------------------------------------------------

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
#include "PowerupPool.h"
#include "Powerup.h"
#include "SoundManager.h"
#include "HeatMapManager.h"
#include "iostream"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace std;
using namespace irrklang;


/// <summary>	Healthbar. </summary>
#define X1BAR 10
#define X2BAR 10
#define Y1BAR 10
#define Y2BAR 25
#define MAX_HEALTH 100

// This is the movement speed in units per second.
#define MOVEMENT_SPEED 50.f
// Correction for rotating the Y Axis on the player object
#define Y_AXIS_CORRECTION -90.f

#define PLAYER_MODEL "../media/Models/player/player2.3ds"
#define GUN_MODEL "../media/Models/weapons/LowPoly_Irrlicht.3ds"
#define GUN_COLOR "../media/Textures/Gun_Color.png"
#define GUN_SHOT_SOUND "../media/Sounds/PlayerGunShot.wav"
#define TAKE_DAMAGE_SOUND "../media/Sounds/takedamage.mp3"

#define VULNERABLE_BASE_TIMER 75
#define BULLET_BASE_TIMER 30

const vector3df bulletSize = vector3df(0.03f, 0.125f, 0.25f);

/// <summary>	The player i device. </summary>
IrrlichtDevice* playerIDevice;
/// <summary>	The player driver. </summary>
IVideoDriver* playerDriver;
/// <summary>	The player smgr. </summary>
ISceneManager* playerSmgr;
/// <summary>	Manager for sound. </summary>
SoundManager* soundManager;
/// <summary>	State of the game over. </summary>
GameOverState gameOverState;
/// <summary>	The player col. </summary>
Collision playerCol;
HeatMapManager* heatMapManager = heatMapManager->GetInstance();

/// <summary>	The player scores. </summary>
Score playerScores;
/// <summary>	The game. </summary>
Game* game;

/// <summary>	The player object. </summary>
ISceneNode* playerObject;
ISceneNode* camFollowObject;
/// <summary>	The gun node. </summary>
ISceneNode* gunNode;
/// <summary>	The health. </summary>
f32 health;
/// <summary>	The time. </summary>
u32 time;
/// <summary>	The current position. </summary>
vector3df currentPosition;

/// <summary>	The mouse position. </summary>
vector3df mousePosition;
/// <summary>	to mouse position. </summary>
vector3df toMousePos;

/// <summary>	The vulnerable timer. </summary>
s32 vulnerableTimer = 0;

/// <summary>	The pool. </summary>
BulletPool* pool;
/// <summary>	The active bullets. </summary>
core::array<Bullet*> activeBullets;

PowerupPool* powPool;

// FRAMEDELTATIME
/// <summary>	The frame delta time. </summary>
f32 frameDeltaTime;
/// <summary>	True if has shot, false if not. </summary>
bool hasShot = false;

///-------------------------------------------------------------------------------------------------
/// <summary>	Constructor. </summary>
///-------------------------------------------------------------------------------------------------

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

///-------------------------------------------------------------------------------------------------
/// <summary>	Initializes this object. </summary>
///-------------------------------------------------------------------------------------------------

void Player::Init()
{
	health = MAX_HEALTH;
	IMesh* playerMesh = playerSmgr->getMesh(PLAYER_MODEL);
	playerObject = playerSmgr->addMeshSceneNode(playerMesh, 0, -1, vector3df(266, 0, 266), vector3df(0, 0, 0));
	camFollowObject = playerSmgr->addCubeSceneNode();
	if (playerObject)
	{
		playerObject->setMaterialFlag(EMF_LIGHTING, true);
	}
	if (camFollowObject) 
	{
		camFollowObject->setParent(playerObject);
		camFollowObject->setScale(vector3df(0,0,0));
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
	powPool = powPool->GetInstance(playerIDevice);

	// Set the timer to the bullet base time
	bulletTimer = BULLET_BASE_TIMER;
}

///-------------------------------------------------------------------------------------------------
/// <summary>	Moves the given input receiver. </summary>
///-------------------------------------------------------------------------------------------------

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

	heatMapManager->AddWeight(heatMapManager->CheckZoneFromPosition(newPosition), frameDeltaTime*2);
	if (heatMapManager->CheckZoneFromPosition(newPosition) == heatMapManager->activeZone && heatMapManager->isPoisonCloudActive) {
		TakeDamage(1);
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

	Powerup* pow = playerCol.CollidesWithPowerup(playerObject);
	if (pow)
	{
		switch (pow->GetPowerupType())
		{
		case 0:
			health += 25;
			if (health > 100)
				health = 100;
			break;
		case 1:
			rapidFireTimer = 1000;
			break;
		case 2:
			splitFireTimer = 1000;
			break;
		}
		powPool->ReturnResource(pow);
		
	}
}

///-------------------------------------------------------------------------------------------------
/// <summary>	Shoots. </summary>
///-------------------------------------------------------------------------------------------------

void Player::Shoot(InputReceiver inputReceiver, EnemySpawner* enemies)
{
	Raycast(inputReceiver.GetMousePosition(), playerSmgr->getActiveCamera());
	if (bulletTimer > 0)
	{
		bulletTimer -= frameDeltaTime;
	}
	if (rapidFireTimer > 0)
	{
		rapidFireTimer -= frameDeltaTime;
	}
	if (splitFireTimer > 0)
	{
		splitFireTimer -= frameDeltaTime;
	}
	if (inputReceiver.GetIsLeftMouseButtonPressed() && bulletTimer <= 0)
	{
		soundManager->PlaySound(GUN_SHOT_SOUND, false);

		// Bullet instance
		Bullet* bullet = pool->GetResource();

		// Main bullet
		bullet->SetBullet(playerSmgr->addCubeSceneNode(10.f, 0, -1, playerObject->getPosition(), playerObject->getRotation(), bulletSize));
		if (bullet->GetBullet())
		{
			bullet->GetBullet()->setVisible(false);
		}

		// Rapid fire active
		if (rapidFireTimer > 0 && splitFireTimer <= 0)
		{
			bullet->SetBulletMode(Bullet::BulletMode::rapidFire);
		}

		// Split fire active
		if (splitFireTimer > 0 && rapidFireTimer <= 0)
		{
			// Bullet left/right instances
			Bullet* leftBullet = pool->GetResource();
			Bullet* rightBullet = pool->GetResource();

			leftBullet->SetBulletMode(Bullet::BulletMode::splitFire);
			rightBullet->SetBulletMode(Bullet::BulletMode::splitFire);
			leftBullet->SetBulletSpread(-0.1f);
			rightBullet->SetBulletSpread(0.1f);

			// Left bullet for split fire
			leftBullet->SetBullet(playerSmgr->addCubeSceneNode(10.f, 0, -1, playerObject->getPosition(), playerObject->getRotation(), bulletSize));
			if (leftBullet->GetBullet())
			{
				leftBullet->GetBullet()->setVisible(false);
			}

			// Right bullet for split fire
			rightBullet->SetBullet(playerSmgr->addCubeSceneNode(10.f, 0, -1, playerObject->getPosition(), playerObject->getRotation(), bulletSize));
			if (rightBullet->GetBullet())
			{
				rightBullet->GetBullet()->setVisible(false);
			}

			// Bullet visibility
			leftBullet->GetBullet()->setVisible(true);
			rightBullet->GetBullet()->setVisible(true);

			// Push bullets to active bullets list
			activeBullets.push_back(leftBullet);
			activeBullets.push_back(rightBullet);
		}

		if (rapidFireTimer > 0 && splitFireTimer > 0)
		{
			Bullet* leftBullet = pool->GetResource();
			Bullet* rightBullet = pool->GetResource();

			bullet->SetBulletMode(Bullet::BulletMode::rapidFire);

			leftBullet->SetBulletMode(Bullet::BulletMode::rapidSplitFire);
			rightBullet->SetBulletMode(Bullet::BulletMode::rapidSplitFire);
			leftBullet->SetBulletSpread(-0.1f);
			rightBullet->SetBulletSpread(0.1f);
			
			// Left bullet for split fire
			leftBullet->SetBullet(playerSmgr->addCubeSceneNode(10.f, 0, -1, playerObject->getPosition(), playerObject->getRotation(), bulletSize));
			if (leftBullet->GetBullet())
			{
				leftBullet->GetBullet()->setVisible(false);
			}

			// Right bullet for split fire
			rightBullet->SetBullet(playerSmgr->addCubeSceneNode(10.f, 0, -1, playerObject->getPosition(), playerObject->getRotation(), bulletSize));
			if (rightBullet->GetBullet())
			{
				rightBullet->GetBullet()->setVisible(false);
			}

			// Bullet visibility
			leftBullet->GetBullet()->setVisible(true);
			rightBullet->GetBullet()->setVisible(true);

			// Push bullets to active bullets list
			activeBullets.push_back(leftBullet);
			activeBullets.push_back(rightBullet);
		}

		// Push bullets to active bullets list
		activeBullets.push_back(bullet);

		hasShot = true;

		// Bullet visibility
		bullet->GetBullet()->setVisible(true);

		// Timer reset
		bulletTimer = bullet->GetBulletTimer();
	}
	if (hasShot)
	{
		if (!activeBullets.empty())
		{
			for (int i = 0; i < activeBullets.size(); i++)
			{
				activeBullets[i]->UpdateBullet(mousePosition, playerObject->getPosition(), frameDeltaTime);
				for (int j = 0; j < enemies->getActiveEnemies().size(); j++)
				{
					if (playerCol.SceneNodeWithSceneNode(enemies->getActiveEnemies()[j]->GetEnemySceneNode(), activeBullets[i]->GetBullet()))
					{
						enemies->GetEnemy(j)->TakeDamage(activeBullets[i]->GetDamage());
						playerScores.DisplayScore(10);
						pool->ReturnResource(activeBullets[i]);
						activeBullets.erase(i);
						// return to correct the index
						return;
					}
				}
				// Check collision with static walls or the life time of the bullet
				if ((activeBullets[i] && playerCol.CollidesWithStaticObjects(activeBullets[i]->GetBullet())) ||
					(activeBullets[i] && activeBullets[i]->GetBulletLifeTimer() == 0))
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

///-------------------------------------------------------------------------------------------------
/// <summary>	Take damage. </summary>
///-------------------------------------------------------------------------------------------------

void Player::TakeDamage(f32 damage)
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

///-------------------------------------------------------------------------------------------------
/// <summary>	Draw health bar. </summary>
///-------------------------------------------------------------------------------------------------

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

///-------------------------------------------------------------------------------------------------
/// <summary>	Gets player object. </summary>
///-------------------------------------------------------------------------------------------------

ISceneNode* Player::getPlayerObject()
{
	return playerObject;
}

irr::scene::ISceneNode * Player::getCamFollowObject()
{
	return camFollowObject;
}

///-------------------------------------------------------------------------------------------------
/// <summary>	Raycasts. </summary>
///-------------------------------------------------------------------------------------------------

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

///-------------------------------------------------------------------------------------------------
/// <summary>	Executes the line intersect action. </summary>
///-------------------------------------------------------------------------------------------------

bool Player::OnLineIntersect(irr::core::plane3df &plane, irr::core::line3df &ray)
{
	return plane.getIntersectionWithLine(ray.start, ray.getVector(), mousePosition);
}

///-------------------------------------------------------------------------------------------------
/// <summary>	Gets vulnerable timer. </summary>
///-------------------------------------------------------------------------------------------------

s32 Player::getVulnerableTimer()
{
	return vulnerableTimer;
}

///-------------------------------------------------------------------------------------------------
/// <summary>	Gets mouse position. </summary>
///-------------------------------------------------------------------------------------------------

vector3df Player::GetMousePosition()
{
	return mousePosition;
}

s32 Player::GetRapidFireTimer()
{
	return rapidFireTimer;
}

s32 Player::GetSplitFireTimer()
{
	return splitFireTimer;
}
