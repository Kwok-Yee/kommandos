#include <irrlicht.h>
#include "EnemyBehaviour.h"
#include "Collision.h"
#include "Player.h"
#include "Score.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;

// This is the movement speed in units per second.
const f32 ENEMY_MOVEMENT_SPEED = 15.f;
const s32 ENEMY_MAX_HEALTH = 100;

IrrlichtDevice* enemyBehaviourIDevice;
IVideoDriver* enemyBevaiourDriver;
ISceneManager* enemyBaviourSmgr;
f32 enemyHealth;

IMeshSceneNode* enemyNode;
Score score;

EnemyBehaviour::EnemyBehaviour(IrrlichtDevice* device) {
	enemyBehaviourIDevice = device;
	enemyBevaiourDriver = enemyBehaviourIDevice->getVideoDriver();
	enemyBaviourSmgr = enemyBehaviourIDevice->getSceneManager();
}

IMeshSceneNode* EnemyBehaviour::Spawn(vector3df startPosition)
{
	enemyHealth = ENEMY_MAX_HEALTH;
	// spawn enemy
	enemyNode = enemyBaviourSmgr->addMeshSceneNode(enemyBaviourSmgr->getMesh("../media/ninja.b3d"));
	if (enemyNode)
	{
		enemyNode->setMaterialFlag(video::EMF_LIGHTING, false);
		enemyNode->setScale(vector3df(10, 10, 10));
		/*enemyNode->setFrameLoop(0, 13); // For walking animation
		enemyNode->setAnimationSpeed(15);*/

		enemyNode->setScale(vector3df(2.f, 2.f, 2.f));
		enemyNode->setRotation(vector3df(0, -90, 0));
		enemyNode->setMaterialTexture(0, enemyBevaiourDriver->getTexture("../media/nskinrd.jpg"));
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
	vector3df delta = playerPosition - enemyPosition; //Save delta
	vector3df deltaNormalized = delta;
	deltaNormalized.normalize(); // If it isnt done in two lines, the delta gets normalized

	enemyNode->setRotation(core::vector3df(0, atan2(deltaNormalized.X, deltaNormalized.Z) * 180 / PI, 0));

	// Change position based on delta and speed
	if (delta.getLength() > vector3df(4, 4, 4).getLength())
	{
		enemyPosition += deltaNormalized * ENEMY_MOVEMENT_SPEED * frameDeltaTime;
		vector3df oldPosition = enemyNode->getPosition();
		enemyNode->setPosition(enemyPosition);

		Collision collision;
		if (collision.CollidesWithStaticObjects(enemyNode))
			enemyNode->setPosition(oldPosition);
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
	return health;
	
	if (health < 0) {
		//score.Scoring(enemyBehaviourIDevice);
		score.DisplayScore(10);
	}
	
}
