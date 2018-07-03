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
#include "HeatMapManager.h"
#include "Game.h"
#include "Camera.h" 
#include <new>  

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
Camera* _cam;
HeatMapManager* heatMapMngr;

core::array<Enemy*> activeEnemies;
irr::core::array<vector3df> spawnPositions;
u32 amountOfEnemies, resize;
u32 currentWave = 0;

ParticleSystem *particleSystem;
const path bloodSplatter = "../media/Textures/bloodNew2.bmp";
u32 prevFrameTime;
EnemySpawner* spawner;

EnemySpawner::EnemySpawner(IrrlichtDevice* device, Player* Player)
{
	particleSystem = new ParticleSystem(device);
	enemySpawnerIDevice = device;
	enemySpawnerSmgr = enemySpawnerIDevice->getSceneManager();
	_player = Player;
	game_EnemySpawner = game_EnemySpawner->GetInstance();
	heatMapMngr = heatMapMngr->GetInstance();
	enemyPool = EnemyPool::GetInstance(device);
	_cam = _cam->GetInstance(NULL);
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
	InitialiseWaveData();
	spawner = this;

	Spawn();
}
//Struct for wavedata for each enemy in wave
struct WaveData
{
	WaveData(int _waveNumber, int _maxRegularEnem, int _maxFastEnem, int _maxTankEnem, int _maxMatroEnem)
	{
		waveNumber = _waveNumber;
		maxRegularEnem = _maxRegularEnem;
		maxFastEnem = _maxFastEnem;
		maxTankEnem = _maxTankEnem;
		maxTankEnem = _maxMatroEnem;
	};
	int waveNumber;
	int maxRegularEnem;
	int maxFastEnem;
	int maxTankEnem;
	int maxMatroEnem;
};
WaveData* waveData[15];

void EnemySpawner::UpdateEnemies()
{
	// Work out a frame delta time.
	const u32 now = enemySpawnerIDevice->getTimer()->getTime();
	const f32 frameDeltaTime = (f32)(now - prevFrameTime) / 1000.f; // Time in seconds
	prevFrameTime = now;
	particleSystem->Update(frameDeltaTime);
	// Update all enemies
	for (int i = 0; i < activeEnemies.size(); i++)
	{
		activeEnemies[i]->Update(frameDeltaTime);

		if (activeEnemies[i]->IsDead())
		{
			particleSystem->CreateParticles(activeEnemies[i]->GetEnemySceneNode()->getPosition(), bloodSplatter);// for creating blood on enemies
			heatMapMngr->AddWeight(heatMapMngr->CheckZoneFromPosition(activeEnemies[i]->GetEnemySceneNode()->getAbsolutePosition()), 5.0f);
			killedEnemies++;
			powerUpSpawner->PowerUpSpawn(activeEnemies[i]->GetEnemySceneNode()->getPosition());
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
		currentWave++;
		Spawn();
		if (currentWave % 3 == 0)
		{
			_cam->state = _cam->bigWaveShaking;
		}
		else
		{
			_cam->state = _cam->waveShaking;
		}
		//Insert pause/UI between waves here
		printf("wave changed");
	}
}
core::array<Enemy*> EnemySpawner::getActiveEnemies() { return activeEnemies; }
Enemy* EnemySpawner::GetEnemy(int id) { return activeEnemies[id]; }
EnemySpawner* EnemySpawner::GetSpawner() { return spawner; }

void EnemySpawner::InitialiseWaveData()
{
	//Amount of enemies:
	waveData[0] = new WaveData(1, 5, 0, 0, 0);//5
	waveData[1] = new WaveData(2, 10, 0, 0, 0);//10
	waveData[2] = new WaveData(3, 8, 3, 0, 0);//11
	waveData[3] = new WaveData(4, 12, 5, 0, 0);//17
	waveData[4] = new WaveData(5, 15, 2, 2, 1);//20
	waveData[5] = new WaveData(6, 10, 0, 7, 1);//18
	waveData[6] = new WaveData(7, 7, 7, 4, 2); //20
	waveData[7] = new WaveData(8, 0, 15, 0, 5); //20
	waveData[8] = new WaveData(9, 0, 0, 15, 7); //22
	waveData[9] = new WaveData(10, 10, 10, 10, 8); //38
	waveData[10] = new WaveData(11, 0, 15, 15, 10); //40
}
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
	for (int i = 0; i < waveData[currentWave]->maxRegularEnem; i++)
	{
		srand(time(NULL) * i);
		u32 randomPos = rand() % 4;

		enemy = enemyPool->GetResource();
		enemy->SetPlayer(_player);
		enemy->SetEnemyType(Enemy::EnemyType::basic);
		enemy->GetEnemySceneNode()->setPosition(spawnPositions[randomPos]);
		collision.AddDynamicToList(enemy->GetEnemySceneNode());
		activeEnemies.push_back(enemy);
	}

	for (int j = 0; j < waveData[currentWave]->maxFastEnem; j++)
	{
		srand(time(NULL) * j);
		u32 randomPos = rand() % 4;

		enemy = enemyPool->GetResource();
		enemy->SetPlayer(_player);
		enemy->SetEnemyType(Enemy::EnemyType::fast);
		enemy->GetEnemySceneNode()->setPosition(spawnPositions[randomPos]);
		collision.AddDynamicToList(enemy->GetEnemySceneNode());
		activeEnemies.push_back(enemy);
	}

	for (int k = 0; k < waveData[currentWave]->maxTankEnem; k++)
	{
		srand(time(NULL) * k);
		u32 randomPos = rand() % 4;

		enemy = enemyPool->GetResource();
		enemy->SetPlayer(_player);
		enemy->SetEnemyType(Enemy::EnemyType::tanky);
		enemy->GetEnemySceneNode()->setPosition(spawnPositions[randomPos]);
		collision.AddDynamicToList(enemy->GetEnemySceneNode());
		activeEnemies.push_back(enemy);
	}

	for (int k = 0; k < waveData[currentWave]->maxMatroEnem; k++)
	{
		srand(time(NULL) * k);
		u32 randomPos = rand() % 4;

		enemy = enemyPool->GetResource();
		enemy->SetPlayer(_player);
		enemy->SetEnemyType(Enemy::EnemyType::matroshka, 2);
		enemy->GetEnemySceneNode()->setPosition(spawnPositions[randomPos]);
		collision.AddDynamicToList(enemy->GetEnemySceneNode());
		activeEnemies.push_back(enemy);
	}
}