#pragma once
#include <irrlicht.h>
#include "EnemySpawner.h"

class Player;

// Movement speed in units per second.
#define ENEMY_MOVEMENT_SPEED 15.f
#define MAX_HEALTH 100
#define MAX_SEE_AHEAD 15
#define MAX_AVOID_FORCE 5
class EnemyBehaviour
{
public:
	irr::f32 health;
	EnemySpawner* spawner;
	// Constructor
	EnemyBehaviour();
	// Initializer
	void Spawn(irr::core::vector3df startPosition);
	// Container method for Move()
	void Update(irr::f32 frameDeltaTime);
	// Returns the health of the enemy that took damage
	void TakeDamage(irr::f32 damage);
	//Resets the path so it can find a new one.
	void ResetPath(irr::core::vector3df playerPosition);
	irr::scene::ISceneNode* GetEnemyObject();
	void Draw();

private:
	// Movement, melee detection, and collision is handled here
	void Move(irr::f32 frameDeltaTime);
	irr::scene::ISceneNode* enemyNode;
	irr::core::vector3df enemyPosition;
	irr::core::vector3df velocity;
	irr::core::vector3df ahead;
	irr::core::vector3df avoidance;
	irr::scene::ISceneNode* threat;
	bool avoid = false;
};
