#include <irrlicht.h>
#include "EnemySpawner.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "ParticleSystem.h"
#include "EnemyBehaviour.h"
#include "Player.h"
#include "Collision.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace std;

const u32 maxWaves = 10;

IrrlichtDevice* enemySpawnerIDevice;
ISceneManager* enemySpawnerSmgr;
EnemyBehaviour* enemyBehaviour;
Player* _player;
Collision collision;

array<vector3df> spawnPositions;
u32 amountOfEnemies;
core::array<IMeshSceneNode*> enemies;
u32 currentWave = 0;

ParticleSystem particle;
const path bloodSplatter = "../media/blood.bmp";
u32 prevFrameTime;

EnemySpawner::EnemySpawner(IrrlichtDevice* device, Player* Player)
{
	enemySpawnerIDevice = device;
	enemySpawnerSmgr = enemySpawnerIDevice->getSceneManager();
	enemyBehaviour = new EnemyBehaviour(enemySpawnerIDevice);
	_player = Player;

	amountOfEnemies = 6;
	//setting spawnpositions in the corners.
	spawnPositions.push_back(vector3df(-82, 0, -78));
	spawnPositions.push_back(vector3df(78, 0, -78));
	spawnPositions.push_back(vector3df(78, 0, 78));
	spawnPositions.push_back(vector3df(-82, 0, 78));


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

	// Update all enemies
	for (int i = 0; i < enemies.size(); i++)
	{
		if (!(_player->vulnerable > 0 && collision.SceneNodeWithSceneNode(_player->getPlayerObject(), enemies[i]))) 
		{
			if (enemyBehaviour->Update(enemies[i], _player->getPlayerObject()->getPosition(), frameDeltaTime))
			{
				_player->TakeDamage(10, frameDeltaTime);
			}
		}

		if (enemyHealthValues[i] <= 0)
		{
			//creates a particle
			particle.hit = true;
			particle.CreateParticle(enemies[i]->getPosition(), bloodSplatter);
			//deletes enemy
			enemySpawnerSmgr->addToDeletionQueue(enemies[i]);
			enemies.erase(i);
			enemyHealthValues.erase(i);
		}
	}

	if (enemies.size() <= 0 && currentWave < maxWaves) {
		Spawn();
		currentWave++;
	}
}

void EnemySpawner::Spawn() {

	for (int i = 0; i < amountOfEnemies; i++)
	{
		srand(time(NULL) * i);
		u32 randomPos = rand() % 4;
		enemyHealthValues.push_back(100);
		enemies.push_back(enemyBehaviour->Spawn(spawnPositions[randomPos]));
	}
}

core::array<IMeshSceneNode*> EnemySpawner::getEnemies() {
	return enemies;
}

EnemyBehaviour* EnemySpawner::getEnemyBehaviour() {
	return enemyBehaviour;
}

