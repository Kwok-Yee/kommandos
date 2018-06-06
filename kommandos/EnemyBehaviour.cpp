#include <irrlicht.h>
#include "EnemyBehaviour.h"
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "Collision.h"
#include "Pathfinding.h"
#include "Player.h"
#include "Game.h"
#include "ParticleSystem.h"
#include "EnemySpawner.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace std;

const vector3df scaleVect = vector3df(2.0f, 2.0f, 2.0f);
const f32 meleeRange = vector3df(4, 4, 4).getLength();
vector3df avoidank;

IrrlichtDevice* enemyBehaviourIDevice;
IVideoDriver* enemyBehaviourDriver;
ISceneManager* enemyBehaviourSmgr;
Player* ePlayer;
Collision col;
Pathfinding* pathfinding;
Game* eGame;
ParticleSystem particle;
const path bloodSplatter = "../media/blood.bmp";

EnemyBehaviour::EnemyBehaviour()
{
	eGame = eGame->GetInstance();
	enemyBehaviourIDevice = eGame->device;
	enemyBehaviourDriver = enemyBehaviourIDevice->getVideoDriver();
	enemyBehaviourSmgr = enemyBehaviourIDevice->getSceneManager();
	pathfinding = new Pathfinding();
	particle.SystemParticle(enemyBehaviourIDevice);
	ePlayer = eGame->player;
}

void EnemyBehaviour::Spawn(vector3df startPosition)
{
	health = MAX_HEALTH;
	enemyPosition = startPosition;
	IMesh* enemyMesh = enemyBehaviourSmgr->getMesh("../media/ninja.b3d");
	enemyNode = enemyBehaviourSmgr->addMeshSceneNode(enemyMesh);
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
	}
	//pathfinding->FindPath(startPosition, ePlayer->getPlayerObject()->getPosition());
}

void EnemyBehaviour::Update(f32 frameDeltaTime)
{
	return Move(frameDeltaTime);
}

void EnemyBehaviour::Move(f32 frameDeltaTime)
{
	if (!enemyNode) return;
	//if (!pathfinding->foundGoal) return;
	// Get position delta compared to player position
	//vector3df enemyPosition = enemyNode->getPosition();
	vector3df delta = ePlayer->getPlayerObject()->getPosition() - enemyPosition; //Save delta
	vector3df enemyMovementNormalized = delta;
	enemyMovementNormalized.normalize();
	velocity = vector3df(0, 0, 0);
	vector3df steering = enemyMovementNormalized;// -velocity;
	enemyBehaviourDriver->setTransform(video::ETS_WORLD, core::IdentityMatrix);
	ahead = enemyPosition +(steering * MAX_SEE_AHEAD);
	//line3df aheadLine = line3df(enemyPosition, ahead);
	threat = col.findMostThreateningObstacle(ahead, enemyPosition);
	avoid = false;
	ahead.Y = 0.4;
	//ISceneNode* treath = col.GetCollidedObjectWithLine(aheadLine);
	if (threat)
	{
		//cout << "avoiding " << threat->getAbsolutePosition().X << " , " << threat->getAbsolutePosition().Z << endl;
		//ISceneNode* treath = col.GetCollidedObjectWithLine(aheadLine);
		avoidank = ahead - threat->getAbsolutePosition();//finds delta between the object and the ahead
		avoidance = avoidank;
		avoidance.normalize();//normalize for the direction.
		//avoidance *= MAX_AVOID_FORCE;
		steering += avoidance;//add it to steering
		steering.normalize();
		avoid = true;
	}

	//velocity += steering;
	//velocity.normalize();
	if (pathfinding->foundGoal)
	{
		//steering = pathfinding->NextPathPos(enemyNode) - enemyPosition;
		//steering.normalize();
	}

	enemyNode->setRotation(vector3df(0, atan2(steering.X, steering.Z) * 180 / PI, 0));

	// Change position based on delta and speed
	if (delta.getLength() > meleeRange) // If farther than melee attack range,
	{
		// Move towards player
		/*if(threat)
			enemyPosition += avoidance * frameDeltaTime * ENEMY_MOVEMENT_SPEED;
		else*/
		enemyPosition += steering * frameDeltaTime * ENEMY_MOVEMENT_SPEED;
		vector3df oldPosition = enemyNode->getPosition();
		enemyNode->setPosition(enemyPosition);

		// Confirm static object collision
		//if (col.CollidesWithStaticObjects(enemyNode))
		//	enemyNode->setPosition(oldPosition);
	}
	else
		ePlayer->TakeDamage(10, frameDeltaTime);
}

void EnemyBehaviour::Draw()
{
	enemyBehaviourDriver->setTransform(video::ETS_WORLD, core::IdentityMatrix);
	enemyBehaviourDriver->draw3DLine(enemyPosition, ahead, SColor(255, 0, 0, 255));
	if (avoid) 
	{
		enemyBehaviourDriver->draw3DLine(enemyPosition, avoidank, SColor(255, 255, 0, 0));
	}
}

void EnemyBehaviour::ResetPath(vector3df playerPosition) {
	pathfinding->initStartGoal = false;
	pathfinding->foundGoal = false;
	pathfinding->FindPath(enemyNode->getPosition(), playerPosition);
}

ISceneNode* EnemyBehaviour::GetEnemyObject() {
	return enemyNode;
}

void EnemyBehaviour::TakeDamage(f32 damage)
{
	if (health > 0)
	{
		health -= damage;
	}
	if (health <= 0)
	{
		//creates a particle
		particle.hit = true;
		particle.CreateParticles(enemyNode->getPosition());// for creating blood on enemies

		spawner->RemoveFromList(this);
	};
}

