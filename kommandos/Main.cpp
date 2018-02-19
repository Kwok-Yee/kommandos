#include <irrlicht.h>

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

vector3df cameraPosition = vector3df(0, 100, 0);
vector3df cameraTarget = vector3df(0, 0, 1);

int main()
{

	IrrlichtDevice *device =
		createDevice(video::EDT_SOFTWARE, dimension2d<u32>(800, 600), 16,
			false, false, false, 0);

	if (!device)
		return 1;

	device->setWindowCaption(L"Hello World! - Irrlicht Engine Demo");
	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();
	IGUIEnvironment* guienv = device->getGUIEnvironment();

	guienv->addStaticText(L"Hello World! This is the Irrlicht Software renderer!",
		rect<s32>(10, 10, 260, 22), true);

	IMesh* playerMesh = smgr->getMesh("../media/Color_Player.3ds");
	if (!playerMesh)
	{
		device->drop();
		return 1;
	}
	IMeshSceneNode* playerNode = smgr->addMeshSceneNode(playerMesh);
	if (playerNode)
	{
		//playerNode->setMaterialFlag(EMF_LIGHTING, false);
		//playerNode->setMD2Animation(scene::EMAT_STAND);
		playerNode->setMaterialTexture(0, driver->getTexture("../media/Color_Player.3ds"));
	}

	ICameraSceneNode* camera = smgr->addCameraSceneNode();

	if (camera)
	{
		camera->setPosition(cameraPosition);
		camera->setTarget(cameraTarget);
	}

	while (device->run())
	{
		driver->beginScene(true, true, SColor(255, 100, 101, 140));

		smgr->drawAll();
		guienv->drawAll();

		driver->endScene();
	}
	device->drop();

	return 0;
}
