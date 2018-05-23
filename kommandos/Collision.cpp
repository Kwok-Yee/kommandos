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

irr::core::array<ISceneNode*> wallList, staticList, dynamicList;

bool Collision::SceneNodeWithSceneNode(irr::scene::ISceneNode* tBox1, irr::scene::ISceneNode* tBox2)
{
	return(tBox1->getTransformedBoundingBox().intersectsWithBox(tBox2->getTransformedBoundingBox()));
}

void Collision::AddWallToList(ISceneNode* wall) { wallList.push_back(wall); }
void Collision::AddStaticToList(ISceneNode* staticObject) { staticList.push_back(staticObject); }
void Collision::AddDynamicToList(ISceneNode* dynamicObject) { dynamicList.push_back(dynamicObject); }

void Collision::RemoveDynamicFromList(ISceneNode* dynamicObject)
{
	for (int i = 0; i < dynamicList.size(); i++)
		if (dynamicList[i] == dynamicObject)
			dynamicList.erase(i);
}

bool Collision::CollidesWithStaticObjects(irr::scene::ISceneNode* dynamicObject)
{
	for (u32 i = 0; i < staticList.size(); i++)
		if (SceneNodeWithSceneNode(dynamicObject, staticList[i]))
			return true;
	return false;
}

void Collision::DiscreteCollisionUpdate()
{
	vector3df difference, seperation;
	//for all dynamic objects
	for (u32 i = 0; i < dynamicList.size(); i++)
	{
		vector3df colliderExtent = dynamicList[i]->getBoundingBox().getExtent() / 2;
		vector3df collPos = dynamicList[i]->getPosition();

		//for all dynamic objects,...
		if (!i == dynamicList.size()) {
			for (u32 j = i++; j < dynamicList.size(); j++)
			{
				//if one overlaps the other
				if (SceneNodeWithSceneNode(dynamicList[i], dynamicList[j]))
				{
					//solve interpenetration
					vector3df dynamicPos = dynamicList[j]->getPosition();
					vector3df dynamicExtent = dynamicList[j]->getBoundingBox().getExtent() / 2;
					vector3df center = collPos + dynamicPos / 2;

					difference = center - collPos;
					seperation = difference.normalize() * (colliderExtent.getLength() + dynamicExtent.getLength() / 2);
					seperation.Y = 0;
					//dissect Y, so it does not average all object heights
					dynamicList[i]->setPosition(center + seperation);

					difference = center - dynamicList[j]->getPosition();
					seperation = difference.normalize() * (colliderExtent.getLength() + dynamicExtent.getLength() / 2);
					seperation.Y = 0;
					//dissect Y, so it does not average all object heights
					dynamicList[j]->setPosition(center + seperation);
				}
			}
		}

		//...static objects...
		for (u32 j = 0; j < staticList.size(); j++)
		{
			//if one overlaps the other
			if (SceneNodeWithSceneNode(dynamicList[i], staticList[j]))
			{
				//solve interpenetration
				vector3df staticExtent = staticList[j]->getBoundingBox().getExtent() / 2;
				vector3df staticPos = staticList[j]->getPosition();
				difference = collPos - staticPos;
				seperation = difference.normalize() * (colliderExtent.getLength() + staticExtent.getLength());
				seperation.Y = 0;

				dynamicList[i]->setPosition(staticPos + seperation);
			}
		}
		//...AND walls
		for (u32 j = 0; j < wallList.size(); j++)
		{
			//if one overlaps the other
			if (SceneNodeWithSceneNode(dynamicList[i], wallList[j]))
			{
				//solve interpenetration
				vector3df wallExtent = wallList[j]->getBoundingBox().getExtent() / 2;

				switch (j)
				{
				case 0: //Left Wall, so move right
					dynamicList[i]->setPosition(
						vector3df(collPos.X, collPos.Y, wallList[j]->getPosition().Z - wallExtent.Z - colliderExtent.Z));
					break;
				case 1: //Top Wall, so move down
					dynamicList[i]->setPosition(
						vector3df(wallList[j]->getPosition().X + wallExtent.X + colliderExtent.X, collPos.Y, collPos.Z));
					break;
				case 2: //Right Wall, so move left
					dynamicList[i]->setPosition(
						vector3df(collPos.X, collPos.Y, wallList[j]->getPosition().Z + wallExtent.Z + colliderExtent.Z));
					break;
				case 3: //Bottom Wall, so move up
					dynamicList[i]->setPosition(
						vector3df(wallList[j]->getPosition().X - wallExtent.X - colliderExtent.X, collPos.Y, collPos.Z));
					break;
				}
			}
		}
	}



}
