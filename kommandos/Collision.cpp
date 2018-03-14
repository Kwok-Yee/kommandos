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

irr::core::array<ISceneNode*> staticList;

bool Collision::SceneNodeWithSceneNode(irr::scene::ISceneNode* tBox1, irr::scene::ISceneNode* tBox2)
{
	return(tBox1->getTransformedBoundingBox().intersectsWithBox(tBox2->getTransformedBoundingBox()));
}

void Collision::AddStaticToList(irr::scene::ISceneNode* staticObject)
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
