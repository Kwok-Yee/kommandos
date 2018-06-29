#include <irrlicht.h>
#include "EnemySpawner.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "ParticleSystem.h"
#include "EnemyBehaviour.h"
#include "Player.h"
#include "Collision.h"
#include "Game.h"
#include <new>  

using namespace irr;
using namespace core;
using namespace scene;
using namespace std;

const u32 maxWaves = 10;

IrrlichtDevice* enemySpawnerIDevice;
ISceneManager* enemySpawnerSmgr;
EnemyBehaviour* enemyBehaviour;
Player* _player;
Game* game_EnemySpawner;
Collision collision;

array<vector3df> spawnPositions;
u32 amountOfEnemies, resize;
core::array<IMeshSceneNode*> enemies;
u32 currentWave = 0;

ParticleSystem *particle;
const path bloodSplatter = "../media/Textures/blood.bmp";
u32 prevFrameTime;



EnemySpawner::EnemySpawner(IrrlichtDevice* device, Player* Player)
{
	particle = new ParticleSystem(device);
	enemySpawnerIDevice = device;
	enemySpawnerSmgr = enemySpawnerIDevice->getSceneManager();
	enemyBehaviour = new EnemyBehaviour(enemySpawnerIDevice);
	_player = Player;
	game_EnemySpawner = game_EnemySpawner->GetInstance();

	amountOfEnemies = 6;
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
//Struct for wavedata for each enemy in wave
struct WaveData
{
	 WaveData(int _waveNumber, int _maxRegularEnem, int _maxFastEnem, int _maxTankEnem)
	{
		waveNumber = _waveNumber;
		maxRegularEnem = _maxRegularEnem;
		maxFastEnem = _maxFastEnem;
		maxTankEnem = _maxTankEnem;
		
	}
	int waveNumber;
	int maxRegularEnem;
	int maxFastEnem;
	int maxTankEnem;
};
WaveData waveData[2];

void EnemySpawner::UpdateEnemies()
{

	// Work out a frame delta time.
	const u32 now = enemySpawnerIDevice->getTimer()->getTime();
	const f32 frameDeltaTime = (f32)(now - prevFrameTime) / 1000.f; // Time in seconds
	prevFrameTime = now;
	particle->Update();
	// Update all enemies
	for (int i = 0; i < enemies.size(); i++)
	{
		if (enemyBehaviour->Update(enemies[i], _player->getPlayerObject()->getPosition(), frameDeltaTime))
		{
			if (!(_player->getVulnerableTimer() > 0))
			{
				_player->TakeDamage(10, frameDeltaTime);
			}
		}

		if (enemyHealthValues[i] <= 0)
		{
			particle->CreateParticles(enemies[i]->getPosition(), bloodSplatter);// for creating blood on enemies
			enemySpawnerSmgr->addToDeletionQueue(enemies[i]);
			collision.RemoveDynamicFromList(enemies[i]);
			enemies.erase(i);
			enemyHealthValues.erase(i);
		}
		if (game_EnemySpawner->GetIsGameOver() == true)
		{
			enemySpawnerSmgr->addToDeletionQueue(enemies[i]);
			enemies.erase(i);
		}
	}

	if (enemies.size() <= 0 && currentWave < maxWaves)
	{
		Spawn();
		currentWave++;
	}
}


void EnemySpawner::InitialiseWaveData()
{
										//Amount of enemies:
	waveData[0] = WaveData(1, 5, 0, 0); //5
	waveData[1] = WaveData(2, 10, 0, 0);//10
	waveData[2] = WaveData(3, 8, 3, 0);//11
	waveData[3] = WaveData(4, 12, 5, 0);//17
	waveData[4] = WaveData(5, 15, 2, 2);//19
	waveData[5] = WaveData(6, 10, 0, 7);//17
	waveData[6] = WaveData(7, 10, 0, 0);//Rest will change depending on yet to be added enemy types
	waveData[7] = WaveData(8, 10, 0, 0);
	waveData[8] = WaveData(9, 10, 0, 0);
	waveData[9] = WaveData(10, 30, 0, 0);
	waveData[10] = WaveData(11, 10, 0, 0);

}

void EnemySpawner::Spawn()
{

	for (int i = 0; i < amountOfEnemies; i++)
	{
		srand(time(NULL) * i);
		u32 randomPos = rand() % 4;
		enemyHealthValues.push_back(100);
		enemies.push_back(enemyBehaviour->Spawn(spawnPositions[randomPos]));
		collision.AddDynamicToList(enemies.getLast());
	}
}

core::array<IMeshSceneNode*> EnemySpawner::getEnemies() { return enemies; }
EnemyBehaviour* EnemySpawner::getEnemyBehaviour() { return enemyBehaviour; }