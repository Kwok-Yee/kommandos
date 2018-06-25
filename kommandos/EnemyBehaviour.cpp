#include <irrlicht.h>
#include <irrKlang.h>
#include "EnemyBehaviour.h"
#include "SoundManager.h"
#include "Collision.h"

#ifndef ENEMYBEHAVIOUR_H //TEST: if works
#include "IEnemyStats.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace irrklang;

// Movement speed in units per second.
#define ENEMY_MOVEMENT_SPEED 15.f;
#define ZOMBIE_TAKE_DAMAGE_SOUND "../media/Sounds/zombiehurt.mp3"
//#define ZOMBIE_DEATH_SOUND "../media/Sounds/zombiedeath.mp3"
const vector3df scaleVect = vector3df(2.0f, 2.0f, 2.0f);
const f32 meleeRange = vector3df(6, 6, 6).getLength();

IrrlichtDevice* enemyBehaviourIDevice;
IVideoDriver* enemyBehaviourDriver;
ISceneManager* enemyBehaviourSmgr;
IMeshSceneNode* enemyNode;
SoundManager* soundManagerEnemy;
Collision col;
IEnemyStats* iEnemyStats;

class FastEnemy : public IEnemyStats {
public:
	//for now it's 2 classes doing the same but you can add for example:
	// speed * health/2 or something in the future with different behaviours maybe
	void setSpeed(float s);
};

class BulkEnemy : public IEnemyStats {
public:
	void setSpeed(float s);
};


int main() {
	FastEnemy fastEnemy;
	BulkEnemy bulkEnemy;

	fastEnemy.setSpeed(20);
	bulkEnemy.setSpeed(10);

	return 0;
}

EnemyBehaviour::EnemyBehaviour(IrrlichtDevice* device) {
	enemyBehaviourIDevice = device;
	enemyBehaviourDriver = enemyBehaviourIDevice->getVideoDriver();
	enemyBehaviourSmgr = enemyBehaviourIDevice->getSceneManager();
	soundManagerEnemy = soundManagerEnemy->GetInstance();
}

IMeshSceneNode* EnemyBehaviour::Spawn(vector3df startPosition)
{

	enemyNode = enemyBehaviourSmgr->addMeshSceneNode(enemyBehaviourSmgr->getMesh("../media/Models/enemy/zombie.3ds"));
	if (enemyNode)
	{
		enemyNode->setMaterialFlag(video::EMF_LIGHTING, false);
		// For walking animation
		//enemyNode->setFrameLoop(0, 13); 
		//enemyNode->setAnimationSpeed(15);

		enemyNode->setScale(scaleVect);
		enemyNode->setRotation(vector3df(0, -90, 0));
		enemyNode->setMaterialTexture(0, enemyBehaviourDriver->getTexture("../media/Textures/zombieskin.png"));
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
		soundManagerEnemy->PlaySound(ZOMBIE_TAKE_DAMAGE_SOUND, false);
		health -= damage;
	}
	else
	{
		//soundManagerEnemy->PlaySound(ZOMBIE_DEATH_SOUND, false);
		health = 0;
	}
	return health;
}

#endif // !ENEMYBEHAVIOUR_H
