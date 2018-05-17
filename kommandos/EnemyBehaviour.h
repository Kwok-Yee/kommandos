#pragma once
#include <irrlicht.h>

class Player;

class EnemyBehaviour
{
public:
	// Constructor
	EnemyBehaviour(irr::IrrlichtDevice* device, Player* player);
	// Initializer
	irr::scene::IMeshSceneNode* Spawn(irr::core::vector3df startPosition);
	// Container method for Move()
	bool Update(irr::scene::IMeshSceneNode* enemyNode, irr::core::vector3df playerPosition, irr::f32 frameDeltaTime);
	// Returns the health of the enemy that took damage
	irr::f32 TakeDamage(irr::f32 damage, irr::f32 health);
	//Resets the path so it can find a new one.
	void ResetPath();

private:
	// Movement, melee detection, and collision is handled here
	bool Move(irr::scene::IMeshSceneNode* enemyNode, irr::core::vector3df playerPosition, irr::f32 frameDeltaTime);
};
