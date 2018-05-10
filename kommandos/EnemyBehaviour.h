#pragma once
#include <irrlicht.h>

class EnemyBehaviour 
{
public:
	EnemyBehaviour(irr::IrrlichtDevice* device);
	irr::scene::IMeshSceneNode* Spawn(irr::core::vector3df startPosition);
	bool Update(irr::scene::IMeshSceneNode* enemyNode, irr::core::vector3df playerPosition, irr::f32 frameDeltaTime);
	irr::f32 TakeDamage(irr::f32 damage, irr::f32 health);
	
private:
	bool Move(irr::scene::IMeshSceneNode* enemyNode, irr::core::vector3df playerPosition, irr::f32 frameDeltaTime);
};