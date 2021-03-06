#include <irrlicht.h>
#include <irrKlang.h>
#include "Enemy.h"
#include "Player.h"
#include "SoundManager.h"
#include "Collision.h"
#include "EnemySpawner.h"
#include "EnemyPool.h"
#include <iostream>

using namespace std;
using namespace irr;
using namespace scene;
using namespace core;

// variables
#define ZOMBIE_TAKE_DAMAGE_SOUND "../media/Sounds/zombiehurt.mp3"
#define ZOMBIE_DEATH_SOUND1 "../media/Sounds/enemy_dmg.mp3"
#define ZOMBIE_DEATH_SOUND2 "../media/Sounds/zombiedeath.mp3"

const f32 meleeRange = vector3df(6, 6, 6).getLength();

vector3df scaleVect;

IrrlichtDevice* enemyIDevice;
IVideoDriver* enemyDriver;
ISceneManager* enemySmgr;
SoundManager* enemySoundManager;
Player* player;
Collision col;

void Enemy::Reset()
{
	SetEnemyType(EnemyType::basic);
	dead = false;
	enemy->setVisible(false);
	enemy->setPosition(vector3df(0, 0, 0));
}

ISceneNode* Enemy::GetEnemySceneNode() { return enemy; }
bool Enemy::IsDead() { return dead; }

void Enemy::SetEnemyType(EnemyType type, int nestAmount)
{
	enemyType = type;
	switch (type)
	{
	case EnemyType::basic:
		enemy->setScale(vector3df(2.0f, 2.0f, 2.0f));
		health = 120;
		speed = 30.f;
		damage = 20.f;
		enemy->setMaterialTexture(0, enemyDriver->getTexture("../media/Textures/zombieskin.png"));
		return;
	case EnemyType::fast:
		enemy->setScale(vector3df(1.7f, 1.7f, 1.7f));
		health = 70;
		speed = 45.f;
		damage = 10.f;
		enemy->setMaterialTexture(0, enemyDriver->getTexture("../media/Textures/fastskin.png"));
		return;
	case EnemyType::tanky:
		enemy->setScale(vector3df(3.0f, 3.0f, 3.0f));
		health = 200;
		speed = 15.f;
		damage = 30.f;
		enemy->setMaterialTexture(0, enemyDriver->getTexture("../media/Textures/tankyskin.png"));
		return;
	case EnemyType::matroshka:
		nestingLvl = nestAmount;
		switch (nestAmount)
		{
		case 2:
			enemy->setScale(vector3df(2.5f, 2.5f, 2.5f));
			health = 160;
			speed = 15.f;
			damage = 20.f;
			enemy->setMaterialTexture(0, enemyDriver->getTexture("../media/Textures/matroshkaskin.png"));
			break;
		case 1:
			enemy->setScale(vector3df(2.0f, 2.0f, 2.0f));
			health = 80;
			speed = 20.f;
			damage = 15.f;
			enemy->setMaterialTexture(0, enemyDriver->getTexture("../media/Textures/tankyskin.png"));
			break;
		case 0:
			enemy->setScale(vector3df(1.5f, 1.5f, 1.5f));
			health = 40;
			speed = 25.f;
			damage = 10.f;
			enemy->setMaterialTexture(0, enemyDriver->getTexture("../media/Textures/fastskin.png"));
			break;
		default:
			break;
		}
		return;
	}
}

void Enemy::SetEnemy(ISceneNode* newEnemy) { enemy = newEnemy; }
void Enemy::SetPlayer(Player* _player) { player = _player; }

void Enemy::Update(float frameDeltaTime)
{
	// Get position delta compared to player position
	vector3df enemyPosition = enemy->getPosition();
	vector3df delta = player->getPlayerObject()->getPosition() - enemyPosition; // Save delta
	vector3df deltaNormalized = delta;
	deltaNormalized.normalize(); // It is done in two lines, so the delta doesnt get normalized

	enemy->setRotation(vector3df(0, atan2(deltaNormalized.X, deltaNormalized.Z) * 180 / PI, 0));

	// Change position based on delta and speed
	if (delta.getLength() > meleeRange) // If farther than melee attack range,
	{
		// Move towards player
		enemyPosition += deltaNormalized * frameDeltaTime * speed;
		vector3df oldPosition = enemy->getPosition();
		enemy->setPosition(enemyPosition);
	}
	else
		Attack();
}

void Enemy::Attack()
{
	if (!(player->getVulnerableTimer() > 0))
	{
		player->TakeDamage(damage);
	}
}

void Enemy::TakeDamage(f32 damage)
{
	if (health > 0) {
		enemySoundManager->PlaySound(ZOMBIE_TAKE_DAMAGE_SOUND, false);
		health -= damage;
	}
	else
	{
		health = 0;
		dead = true;
		switch (rand() % 2) 
		{
		case 0:
			enemySoundManager->PlaySound(ZOMBIE_DEATH_SOUND1, false);
			break;
		case 1:
			enemySoundManager->PlaySound(ZOMBIE_DEATH_SOUND2, false);
			break;
		}

		if (enemyType == EnemyType::matroshka && nestingLvl > 0) 
		{
			EnemySpawner* espawner = espawner->GetSpawner();
			for (int i = 0; i < 2; i++) 
			{
				espawner->SpawnMathroskaMinion(GetEnemySceneNode()->getPosition(), EnemyType::matroshka,nestingLvl-1);
			}
		}
	}
}

Enemy::Enemy(IrrlichtDevice* device)
{
	enemyIDevice = device;
	enemyDriver = enemyIDevice->getVideoDriver();
	enemySmgr = enemyIDevice->getSceneManager();
	enemySoundManager = enemySoundManager->GetInstance();

	dead = false;
	enemy = 0;
	enemy = enemySmgr->addMeshSceneNode(enemySmgr->getMesh("../media/Models/enemy/zombie.3ds"));
	if (enemy)
	{
		enemy->setMaterialFlag(video::EMF_LIGHTING, false);

		enemy->setScale(scaleVect);
		enemy->setRotation(vector3df(0, -90, 0));
		enemy->setMaterialTexture(0, enemyDriver->getTexture("../media/Textures/zombieskin.png"));
	}
	SetEnemyType(EnemyType::basic);
}
