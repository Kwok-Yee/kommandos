#pragma once
#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace scene;

class EnemyBehaviour 
{
public:

	IMeshSceneNode* Spawn(IrrlichtDevice* device, vector3df startPosition);
	void Update(IMeshSceneNode* enemyNode, vector3df playerPosition, f32 frameDeltaTime);

private:

	void Move(IMeshSceneNode* enemyNode, vector3df playerPosition, f32 frameDeltaTime);
	void Attack();

};