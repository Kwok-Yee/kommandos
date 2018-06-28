#include <irrlicht.h>
#include "EnemySpawner.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "ParticleSystem.h"
#include "Enemy.h"
#include "EnemyPool.h"
#include "Player.h"
#include "Collision.h"
#include "Game.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace std;

const u32 maxWaves = 10;

IrrlichtDevice* enemySpawnerIDevice;
ISceneManager* enemySpawnerSmgr;
Enemy* enemy;
Player* _player;
Game* game_EnemySpawner;
EnemyPool* enemyPool;
Collision collision;

core::array<Enemy*> activeEnemies;
irr::core::array<vector3df> spawnPositions;
u32 amountOfEnemies, resize;
u32 currentWave = 0;

ParticleSystem *particle;
const path bloodSplatter = "../media/Textures/blood.bmp";
u32 prevFrameTime;

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

	Spawn();
}

void EnemySpawner::UpdateEnemies()
{
	// Work out a frame delta time.
	const u32 now = enemySpawnerIDevice->getTimer()->getTime();
	const f32 frameDeltaTime = (f32)(now - prevFrameTime) / 1000.f; // Time in seconds
	prevFrameTime = now;
	particle->Update();
	// Update all enemies
	for (int i = 0; i < activeEnemies.size(); i++)
	{
		activeEnemies[i]->Update(frameDeltaTime);

		if (activeEnemies[i]->IsDead())
		{
			particle->CreateParticles(activeEnemies[i]->GetEnemySceneNode()->getPosition(), bloodSplatter);// for creating blood on enemies
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

void EnemySpawner::Spawn()
{
	for (int i = 0; i < amountOfEnemies; i++)
	{
		srand(time(NULL) * i);
		u32 randomPos = rand() % 4;
		u32 enemyType = rand() % 3;

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
		}

		enemy->GetEnemySceneNode()->setPosition(spawnPositions[randomPos]);
		collision.AddDynamicToList(enemy->GetEnemySceneNode());
		activeEnemies.push_back(enemy);
	}
}

core::array<Enemy*> EnemySpawner::getActiveEnemies() { return activeEnemies; }
Enemy* EnemySpawner::GetEnemy(int id) { return activeEnemies[id]; }