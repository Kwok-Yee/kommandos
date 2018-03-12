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

Collision::Collision()
{
}

Collision::~Collision()
{
}

bool Collision::SceneNodeWithSceneNode(const ISceneNode* tBox1, const ISceneNode* tBox2)
{
	return(tBox1->getTransformedBoundingBox().intersectsWithBox(tBox2->getTransformedBoundingBox()));
}
