#include <irrlicht.h>
#include "Collision.h"

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

int main()
{
	Collision col;
	Collision::TAABB box1;
	Collision::TAABB box2;
	

	IrrlichtDevice *device =
		createDevice(video::EDT_SOFTWARE, dimension2d<u32>(800, 600), 16,
			false, false, false, 0);

	if (!device)
		return 1;

	device->setWindowCaption(L"Hello World! - Irrlicht Engine Demo");
	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();
	IGUIEnvironment* guienv = device->getGUIEnvironment();
	IMeshSceneNode* Cube1 = smgr->addCubeSceneNode();
	IMeshSceneNode* Cube2 = smgr->addCubeSceneNode();
	Cube2->setPosition(vector3df{ -15,-15,-15 });
	box1.m_size = vector3df{10,10,10};
	box2.m_size = vector3df{ 10,10,10 };

	//guienv->addStaticText(L"Hello World! This is the Irrlicht Software renderer!",
		//rect<s32>(10, 10, 260, 22), true);
	

	IAnimatedMesh* mesh = smgr->getMesh("../media/sydney.md2");
	if (!mesh)
	{
		device->drop();
		return 1;
	}
	IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode(mesh);

	if (node)
	{
		node->setMaterialFlag(EMF_LIGHTING, false);
		node->setMD2Animation(scene::EMAT_STAND);
		node->setMaterialTexture(0, driver->getTexture("../media/sydney.bmp"));
	}
	smgr->addCameraSceneNode(0, vector3df(0, 30, -40), vector3df(0, 5, 0));

	while (device->run())
	{
		driver->beginScene(true, true, SColor(255, 100, 101, 140));
		if (col.AABBtoAABB(box1, box2)) {
			guienv->addStaticText(L"FOUND COLLISION!",
				rect<s32>(10, 10, 260, 22), true);
		}
		else {
			guienv->addStaticText(L"NOTHING!",
				rect<s32>(10, 10, 260, 22), true);
		}
		smgr->drawAll();
		guienv->drawAll();

		

		driver->endScene();
	}
	device->drop();

	return 0;
}
