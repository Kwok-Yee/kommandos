#include <irrlicht.h>
#include "EnemySpawner.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include "ParticleSystem.h"
#include "Enemy.h"
#include "EnemyPool.h"
#include "Player.h"
#include "Collision.h"
#include "PowerUpSpawner.h"
#include "Game.h"
#include "HeatMapManager.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace std;

const u32 maxWaves = 10;
int killedEnemies = 0;

IrrlichtDevice* enemySpawnerIDevice;
ISceneManager* enemySpawnerSmgr;
Enemy* enemy;
Player* _player;
Game* game_EnemySpawner;
PowerUpSpawner* powerUpSpawner;
EnemyPool* enemyPool;
Collision collision;
HeatMapManager* heatMapMngr = heatMapMngr->GetInstance();

core::array<Enemy*> activeEnemies;
irr::core::array<vector3df> spawnPositions;
u32 amountOfEnemies, resize;
u32 currentWave = 0;

ParticleSystem *particle;
const path bloodSplatter = "../media/Textures/blood.bmp";
u32 prevFrameTime;
EnemySpawner* spawner;

EnemySpawner::EnemySpawner(IrrlichtDevice* device, Player* Player)
{
	particle = new ParticleSystem(device);
	enemySpawnerIDevice = device;
	enemySpawnerSmgr = enemySpawnerIDevice->getSceneManager();
	_player = Player;
	game_EnemySpawner = game_EnemySpawner->GetInstance();
	enemyPool = EnemyPool::GetInstance(device);

	amountOfEnemies = 12;
	resize = 2;
	//setting spawnpositions in the corners.
	spawnPositions.push_back(vector3df(-82, 0, -78) * resize);
	spawnPositions.push_back(vector3df(78, 0, -78) * resize);
	spawnPositions.push_back(vector3df(78, 0, 78) * resize);
	spawnPositions.push_back(vector3df(-82, 0, 78) * resize);


	// In order to do framerate independent movement, we have to know
	// how long it was since the last frame
	prevFrameTime = enemySpawnerIDevice->getTimer()->getTime();

	spawner = this;

	Spawn();
}

void EnemySpawner::UpdateEnemies()
{
	// Work out a frame delta time.
	const u32 now = enemySpawnerIDevice->getTimer()->getTime();
	const f32 frameDeltaTime = (f32)(now - prevFrameTime) / 1000.f; // Time in seconds
	prevFrameTime = now;
	particle->Update(frameDeltaTime);
	// Update all enemies
	for (int i = 0; i < activeEnemies.size(); i++)
	{
		activeEnemies[i]->Update(frameDeltaTime);

		if (activeEnemies[i]->IsDead())
		{
      heatMapMngr->AddWeight(heatMapMngr->CheckZoneFromPosition(enemies[i]->getAbsolutePosition()), 5.0f);
			particle->CreateParticles(activeEnemies[i]->GetEnemySceneNode()->getPosition(), bloodSplatter);// for creating blood on enemies
      killedEnemies++;
      powerUpSpawner->PowerUpSpawn(enemies[i]->getPosition());
			enemyPool->ReturnResource(activeEnemies[i]);
			collision.RemoveDynamicFromList(activeEnemies[i]->GetEnemySceneNode());
			activeEnemies.erase(i);
		}

		if (game_EnemySpawner->GetIsGameOver())
		{
			enemySpawnerSmgr->addToDeletionQueue(activeEnemies[i]->GetEnemySceneNode());
			activeEnemies.erase(i);
		}
	}

	if (activeEnemies.size() <= 0 && currentWave < maxWaves)
	{
		Spawn();
		currentWave++;
	}
}
core::array<Enemy*> EnemySpawner::getActiveEnemies() { return activeEnemies; }
Enemy* EnemySpawner::GetEnemy(int id) { return activeEnemies[id]; }
EnemySpawner* EnemySpawner::GetSpawner() { return spawner; }

void EnemySpawner::SpawnEnemy(vector3df spawnPos, Enemy::EnemyType enemyType, s32 nestAmount)
{
	enemy = enemyPool->GetResource();
	enemy->SetPlayer(_player);
	enemy->SetEnemyType(enemyType, nestAmount);
	enemy->GetEnemySceneNode()->setPosition(spawnPos);
	collision.AddDynamicToList(enemy->GetEnemySceneNode());
	activeEnemies.push_back(enemy);
}

void EnemySpawner::Spawn()
{
	for (int i = 0; i < amountOfEnemies; i++)
	{
		srand(time(NULL) * i);
		u32 randomPos = rand() % 4;
		u32 enemyType = rand() % 4;

		enemy = enemyPool->GetResource();
		enemy->SetPlayer(_player);
		switch (enemyType) {
		case 0:
			enemy->SetEnemyType(Enemy::EnemyType::basic);
			break;
		case 1:
			enemy->SetEnemyType(Enemy::EnemyType::fast);
			break;
		case 2:
			enemy->SetEnemyType(Enemy::EnemyType::tanky);
			break;
		case 3:
			enemy->SetEnemyType(Enemy::EnemyType::matroshka, 2);
			break;
		default:
			enemy->SetEnemyType(Enemy::EnemyType::basic);
			break;
		}

		enemy->GetEnemySceneNode()->setPosition(spawnPositions[randomPos]);
		collision.AddDynamicToList(enemy->GetEnemySceneNode());
		activeEnemies.push_back(enemy);
	}
}

