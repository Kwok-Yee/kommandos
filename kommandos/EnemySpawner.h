#pragma once
#include <irrlicht.h>
#include <list>

class EnemyBehaviour;
class Player;

#define MAX_WAVES 10
#define AMOUNT_OF_ENEMIES 1
class EnemySpawner
{
public:
	EnemySpawner();
	void UpdateEnemies();
	//retruns a list of all enemy objects in the scene
	std::list<EnemyBehaviour*> GetEnemies();
	void RemoveFromList(EnemyBehaviour* enemy);
private:
	//Spawns a enemy at a random spaw position
	void Spawn();
};