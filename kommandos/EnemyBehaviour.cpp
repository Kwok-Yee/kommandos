#include <irrlicht.h>
#include "EnemyBehaviour.h"
#include "Collision.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;

// Movement speed in units per second.
#define ENEMY_MOVEMENT_SPEED 15.f;
const vector3df scaleVect = vector3df(2.0f, 2.0f, 2.0f);
const f32 meleeRange = vector3df(6, 6, 6).getLength();

IrrlichtDevice* enemyBehaviourIDevice;
IVideoDriver* enemyBehaviourDriver;
ISceneManager* enemyBehaviourSmgr;
IMeshSceneNode* enemyNode;
Collision col;

EnemyBehaviour::EnemyBehaviour(IrrlichtDevice* device) {
	enemyBehaviourIDevice = device;
	enemyBehaviourDriver = enemyBehaviourIDevice->getVideoDriver();
	enemyBehaviourSmgr = enemyBehaviourIDevice->getSceneManager();
}

IMeshSceneNode* EnemyBehaviour::Spawn(vector3df startPosition)
{
	enemyNode = enemyBehaviourSmgr->addMeshSceneNode(enemyBehaviourSmgr->getMesh("../media/ninja.b3d"));
	if (enemyNode)
	{
		enemyNode->setMaterialFlag(video::EMF_LIGHTING, false);
		// For walking animation
		//enemyNode->setFrameLoop(0, 13); 
		//enemyNode->setAnimationSpeed(15);

		enemyNode->setScale(scaleVect);
		enemyNode->setRotation(vector3df(0, -90, 0));
		enemyNode->setMaterialTexture(0, enemyBehaviourDriver->getTexture("../media/nskinrd.jpg"));
		enemyNode->setPosition(startPosition);
		return enemyNode;
	}
	return NULL;
}

bool EnemyBehaviour::Update(IMeshSceneNode* enemyNode, vector3df playerPosition, f32 frameDeltaTime)
{
	return Move(enemyNode, playerPosition, frameDeltaTime);
}

bool EnemyBehaviour::Move(IMeshSceneNode* enemyNode, vector3df playerPosition, f32 frameDeltaTime)
{
	// Get position delta compared to player position
	vector3df enemyPosition = enemyNode->getPosition();
	vector3df delta = playerPosition - enemyPosition; // Save delta
	vector3df deltaNormalized = delta;
	deltaNormalized.normalize(); // It is done in two lines, so the delta doesnt get normalized

	enemyNode->setRotation(vector3df(0, atan2(deltaNormalized.X, deltaNormalized.Z) * 180 / PI, 0));

	// Change position based on delta and speed
	if (delta.getLength() > meleeRange) // If farther than melee attack range,
	{
		// Move towards player
		enemyPosition += deltaNormalized * frameDeltaTime * ENEMY_MOVEMENT_SPEED;
		vector3df oldPosition = enemyNode->getPosition();
		enemyNode->setPosition(enemyPosition);

		// Confirm static object collision
		/*if (col.CollidesWithStaticObjects(enemyNode))
			enemyNode->setPosition(oldPosition);*/
		return false;
	}
	else
		return true;
}

f32 EnemyBehaviour::TakeDamage(f32 damage, f32 health)
{
	if (health > 0) {
		health -= damage;
	}
	else health = 0;
	return health;
}
