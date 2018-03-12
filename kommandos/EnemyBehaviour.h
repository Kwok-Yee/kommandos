#pragma once
#include <irrlicht.h>
class EnemyBehaviour {
public:
	irr::scene::IMeshSceneNode* Spawn(irr::IrrlichtDevice* device, irr::core::vector3df startPosition);
	void Update(irr::scene::IMeshSceneNode* enemyNode, irr::core::vector3df playerPosition, irr::f32 frameDeltaTime);
private:
	void Move(irr::scene::IMeshSceneNode* enemyNode, irr::core::vector3df playerPosition, irr::f32 frameDeltaTime);
	void Attack();
};