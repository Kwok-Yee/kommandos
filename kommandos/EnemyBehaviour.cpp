#include <irrlicht.h>
#include "EnemyBehaviour.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;

// This is the movement speed in units per second.
const f32 ENEMY_MOVEMENT_SPEED = 15.f;
scene::IMeshSceneNode* enemyNode;

irr::scene::IMeshSceneNode* EnemyBehaviour::Spawn(IrrlichtDevice* device, vector3df startPosition)
{
	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();

	// spawn enemy
	enemyNode = smgr->addMeshSceneNode(smgr->getMesh("../media/ninja.b3d"));
	if (enemyNode)
	{
		enemyNode->setMaterialFlag(video::EMF_LIGHTING, false);
		enemyNode->setScale(vector3df(10, 10, 10));
		/*enemyNode->setFrameLoop(0, 13); // For walking animation
		enemyNode->setAnimationSpeed(15);*/

		enemyNode->setScale(core::vector3df(1.f, 1.f, 1.f));
		enemyNode->setRotation(core::vector3df(0, -90, 0));
		enemyNode->setMaterialTexture(0, driver->getTexture("../media/nskinrd.jpg"));
		enemyNode->setPosition(startPosition);
		return enemyNode;
	}
	return NULL;
}

void EnemyBehaviour::Update(IMeshSceneNode* enemyNode, vector3df playerPosition, f32 frameDeltaTime) 
{
	Move(enemyNode, playerPosition, frameDeltaTime);
}

void EnemyBehaviour::Move(IMeshSceneNode* enemyNode, vector3df playerPosition, f32 frameDeltaTime)
{
	// Get position delta compared to player position
	vector3df enemyPosition = enemyNode->getPosition();
	vector3df delta = playerPosition - enemyPosition; //Save delta
	vector3df deltaNormalized = delta;
	deltaNormalized.normalize(); // If it isnt done in two lines, the delta gets normalized

								 // Change position based on delta and speed
	if (delta.getLength() > vector3df(5, 5, 5).getLength())
	{
		enemyPosition += deltaNormalized * ENEMY_MOVEMENT_SPEED * frameDeltaTime;
		enemyNode->setPosition(enemyPosition);
	}

	//set object
	enemyNode->setRotation(core::vector3df(0, atan2(deltaNormalized.X, deltaNormalized.Z) * 180 / PI, 0));
}

void EnemyBehaviour::Attack() {

}
