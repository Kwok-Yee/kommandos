#include <irrlicht.h>
#include "EnemySpawner.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <list>
#include <iostream>
#include "EnemyBehaviour.h"
#include "Player.h"
#include "Collision.h"
#include "Game.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace std;

IrrlichtDevice* enemySpawnerIDevice;
ISceneManager* enemySpawnerSmgr;
Game* game;
Player* _player;
Collision collision;

core::array<vector3df> spawnPositions;
std::list<EnemyBehaviour*> enemies;
u32 currentWave = 0;

u32 prevFrameTime;
u32 prevTime;


EnemySpawner::EnemySpawner()
{
	game = game->GetInstance();
	enemySpawnerIDevice = game->device;
	enemySpawnerSmgr = enemySpawnerIDevice->getSceneManager();
	_player = game->player;
	
	//setting spawnpositions in the corners.
	spawnPositions.push_back(vector3df(-79, 0, -79));
	spawnPositions.push_back(vector3df(79, 0, -79));
	spawnPositions.push_back(vector3df(79, 0, 79));
	spawnPositions.push_back(vector3df(-79, 0, 79));


	// In order to do framerate independent movement, we have to know
	// how long it was since the last frame
	prevFrameTime = enemySpawnerIDevice->getTimer()->getTime();
	prevTime = prevFrameTime;

	Spawn();
}

void EnemySpawner::UpdateEnemies() 
{
	// Work out a frame delta time.
	const u32 now = enemySpawnerIDevice->getTimer()->getTime();
	const f32 frameDeltaTime = (f32)(now - prevFrameTime) / 1000.f; // Time in seconds
	prevFrameTime = now;

	/*if (now >= prevTime + (frameDeltaTime * 80000)) {
		for each (EnemyBehaviour* enemy in enemies)
		{
			enemy->ResetPath(_player->getPlayerObject()->getPosition());
		}
	prevTime = now;
	}*/


	// Update all enemies
	for each (EnemyBehaviour* enemy in enemies)
	{
		if (!(_player->vulnerable > 0 && collision.SceneNodeWithSceneNode(_player->getPlayerObject(), enemy->GetEnemyObject())))
		{
			enemy->Update(frameDeltaTime);
		}
	}
	//after all enemies are dead spawn new enemies
	if (enemies.size() <= 0 && currentWave < MAX_WAVES) {
		Spawn();
		currentWave++;
	}
}

void EnemySpawner::Draw() 
{// Work out a frame delta time.
	const u32 now = enemySpawnerIDevice->getTimer()->getTime();
	const f32 frameDeltaTime = (f32)(now - prevFrameTime) / 1000.f; // Time in seconds
	prevFrameTime = now;
	for each (EnemyBehaviour* enemy in enemies)
	{
		enemy->Draw();
	}
}

void EnemySpawner::Spawn() 
{
	for (int i = 0; i < AMOUNT_OF_ENEMIES; i++)
	{
		srand(time(NULL) * i);
		u32 randomPos = rand() % 4; 
		EnemyBehaviour* newEnemy = new EnemyBehaviour();
		newEnemy->spawner = this;
		//newEnemy->Init();
		newEnemy->Spawn(spawnPositions[randomPos]);
		//newEnemy->ResetPath(_player->getPlayerObject()->getPosition());
		enemies.push_back(newEnemy);
	}
}

void EnemySpawner::RemoveFromList(EnemyBehaviour* enemy) 
{
	enemies.remove(enemy);
	enemySpawnerSmgr->addToDeletionQueue(enemy->GetEnemyObject());
}

std::list<EnemyBehaviour*> EnemySpawner::GetEnemies() {
	return enemies;
}


