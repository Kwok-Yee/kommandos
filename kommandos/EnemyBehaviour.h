#pragma once
#include <irrlicht.h>
#include "Player.h"

using namespace irr;
using namespace core;
using namespace scene;

class EnemyBehaviour 
{
public:

	IMeshSceneNode* Spawn(IrrlichtDevice* device, vector3df startPosition);
	bool Update(IMeshSceneNode* enemyNode, vector3df playerPosition, f32 frameDeltaTime);
	bool TakeDamage(f32 damage);
	
private:

	bool Move(IMeshSceneNode* enemyNode, vector3df playerPosition, f32 frameDeltaTime);
};