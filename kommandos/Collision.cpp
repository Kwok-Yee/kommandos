#include "Collision.h"
#include "irrlicht.h"
#include <cmath>

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

core::array<ISceneNode*> staticList;

bool Collision::SceneNodeWithSceneNode(ISceneNode* tBox1, ISceneNode* tBox2)
{
	return(tBox1->getTransformedBoundingBox().intersectsWithBox(tBox2->getTransformedBoundingBox()));
}

void Collision::AddStaticToList(ISceneNode* staticObject)
{
	staticList.push_back(staticObject);
}

bool Collision::CollidesWithStaticObjects(irr::scene::ISceneNode* dynamicObject)
{
	for (u32 i = 0; i < staticList.size(); i++)
		if (SceneNodeWithSceneNode(dynamicObject, staticList[i]))
			return true;
	return false;
}

bool Collision::LineCollidesWithStaticObjects(line3df line)
{
	for (u32 i = 0; i < staticList.size(); i++)
		if (staticList[i]->getBoundingBox().intersectsWithLine(line))
			return true;
	return false;
}

ISceneNode* Collision::GetCollidedObjectWithLine(line3df line)
{
	for (u32 i = 0; i < staticList.size(); i++) {
		if (staticList[i]->getBoundingBox().intersectsWithLine(line))
			return staticList[i];
	}
	return false;
}
