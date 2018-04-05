#include "EnemyBehaviour.h"
#include "EnemySpawner.h"
#include <irrlicht.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using namespace irr;
using namespace core;
using namespace std;

IrrlichtDevice* iDecice;
EnemyBehaviour enemyBehaviour;
array<vector3df> spawnPositions;
u32 amountOfEnemies;

EnemySpawner::EnemySpawner(IrrlichtDevice* device)
{
	iDecice = device;

	amountOfEnemies = 6;
	//setting spawnpositions in the corners.
	spawnPositions.push_back(vector3df(-85, 0, -80));
	spawnPositions.push_back(vector3df(80, 0, -80));
	spawnPositions.push_back(vector3df(80, 0, 80));
	spawnPositions.push_back(vector3df(-85, 0, 80));

	EnemySpawner::Spawn();
}

void EnemySpawner::Spawn() {
	for (int i = 0; i < amountOfEnemies; i++)
	{
		srand(time(NULL) * i);
		u32 randomPos = rand() % 4;
		enemyBehaviour.Spawn(iDecice, spawnPositions[randomPos]);
	}
}

