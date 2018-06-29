#include "Collision.h"
#include "irrlicht.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif	

irr::core::array<ISceneNode*> wallList, staticList, dynamicList, pickupList;

bool Collision::SceneNodeWithSceneNode(irr::scene::ISceneNode* tBox1, irr::scene::ISceneNode* tBox2)
{
	return(tBox1->getTransformedBoundingBox().intersectsWithBox(tBox2->getTransformedBoundingBox()));
}

void Collision::AddWallToList(ISceneNode* wall) { wallList.push_back(wall); }
void Collision::AddStaticToList(ISceneNode* staticObject) { staticList.push_back(staticObject); }
void Collision::AddDynamicToList(ISceneNode* dynamicObject) { dynamicList.push_back(dynamicObject); }
void Collision::AddPickupToList(ISceneNode* playerNode) { pickupList.push_back(playerNode); }

void Collision::RemoveDynamicFromList(ISceneNode* dynamicObject)
{
	for (int i = 0; i < dynamicList.size(); i++)
		if (dynamicList[i] == dynamicObject)
		{
			dynamicList.erase(i);
			break;
		}
}

bool Collision::CollidesWithStaticObjects(irr::scene::ISceneNode* dynamicObject)
{
	for (u32 i = 0; i < staticList.size(); i++)
		if (SceneNodeWithSceneNode(dynamicObject, staticList[i]))
			return true;
	return false;
}


bool Collision::CollidesWithPickupAble(irr::scene::ISceneNode* playerNode)
{
	for (u32 i = 0; i < pickupList.size(); i++)
		if (SceneNodeWithSceneNode(playerNode, pickupList[i]))
		{
			return true;
			pickupList.erase(i);
		}
			
	return false;
}

void Collision::DiscreteCollisionUpdate(f32 frameDeltaTime)
{
	vector3df difference, seperation;
	//for all dynamic objects
	for (u32 i = 0; i < dynamicList.size(); i++)
	{
		vector3df colliderExtent = dynamicList[i]->getBoundingBox().getExtent() / 2;
		vector3df colliderPos = dynamicList[i]->getPosition();

		//for all dynamic objects,...
		for (u32 j = i + 1; j < dynamicList.size(); j++)
		{
			if (j > dynamicList.size())
				break;
			//if one overlaps the other
			if (SceneNodeWithSceneNode(dynamicList[i], dynamicList[j]))
			{
				//solve interpenetration
				vector3df dynamicPos = dynamicList[j]->getPosition();
				vector3df dynamicExtent = dynamicList[j]->getBoundingBox().getExtent() / 2;

				vector3df center;
				if (colliderPos == dynamicPos) //if they are on top of eachother
					center = colliderPos + vector3df(1, 0, 1); //seperate at a random vector
				else
					center = (colliderPos + dynamicPos) / 2; //average of both positions

				difference = center - colliderPos;
				seperation = difference.normalize() * (colliderExtent.getLength() + dynamicExtent.getLength())* frameDeltaTime;
				seperation.Y = 0;
				//dissect Y, so it does not average all object heights
				dynamicList[j]->setPosition(dynamicPos + (seperation));
			}
		}

		//...AND static objects
		for (u32 j = 0; j < staticList.size(); j++)
		{
			//if one overlaps the other
			if (SceneNodeWithSceneNode(dynamicList[i], staticList[j]))
			{
				//solve interpenetration
				vector3df staticExtent = staticList[j]->getBoundingBox().getExtent() / 2;
				vector3df staticPos = staticList[j]->getPosition();
				difference = colliderPos - staticPos;
				seperation = difference.normalize() * (colliderExtent.getLength() + staticExtent.getLength()) / 20;
				seperation.Y = 0;

				dynamicList[i]->setPosition(colliderPos + seperation);
			}
		}
		for (u32 j = 0; j < wallList.size(); j++)
		{

			//if one overlaps the other
			if (SceneNodeWithSceneNode(dynamicList[i], wallList[j]))
			{
				switch (j) {
				case 0: // left wall
					seperation = vector3df(0, 0, -100) * frameDeltaTime;
					dynamicList[i]->setPosition(colliderPos + seperation);
					break;
				case 1: // top wall
					seperation = vector3df(-100, 0, 0) * frameDeltaTime;
					dynamicList[i]->setPosition(colliderPos + seperation);
					break;
				case 2: // right wall
					seperation = vector3df(0, 0, 100) * frameDeltaTime;
					dynamicList[i]->setPosition(colliderPos + seperation);
					break;
				case 3: // bottom wall
					seperation = vector3df(100, 0, 0) * frameDeltaTime;
					dynamicList[i]->setPosition(colliderPos + seperation);
					break;
				}
			}
		}
	}
}
