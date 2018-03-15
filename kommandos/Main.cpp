#include <irrlicht.h>
#include "Collision.h"
#include "driverChoice.h"
#include "InputReceiver.h"
#include "Player.h"
#include <ILogger.h>

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;
using namespace std;

#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(linker, "/subsystem:console /ENTRY:mainCRTStartup")
#endif

const vector3df cameraPosition = vector3df(0, 120, 0);
const vector3df cameraTarget = vector3df(0, 0, 0);

// Initialize the paths for the object its textures
const path crateDiffuse = "../media/crate/crate_diffuse.png";
const path crateNormal = "../media/crate/crate_normal.png";

int main()
{
	Collision collision;
	// Instance of inputReceiver
	InputReceiver inputReceiver;

	// Create device
	IrrlichtDevice* device = createDevice(video::EDT_DIRECT3D9,
		dimension2d<u32>(800, 600), 16, false, false, false, &inputReceiver);

	// No device found
	if (!device) {
		return 1;
	}
	Player* player = new Player(device);

	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();
	IGUIEnvironment* guienv = device->getGUIEnvironment();

	IMesh* planeMesh = smgr->getMesh("../media/ArenaColor.3ds");
	IMeshSceneNode* planeNode = smgr->addMeshSceneNode(planeMesh);
	planeNode->setMaterialFlag(video::EMF_LIGHTING, true);

	IMesh* longWallMeshRight = smgr->getMesh("../media/LongWall.3ds");
	IMeshSceneNode* longWallNodeRight = smgr->addMeshSceneNode(longWallMeshRight);
	longWallNodeRight->setMaterialFlag(video::EMF_LIGHTING, true);
	longWallNodeRight->setPosition(core::vector3df(0, 0, -75));

	IMesh* longWallMeshLeft = smgr->getMesh("../media/LongWall.3ds");
	IMeshSceneNode* longWallNodeLeft = smgr->addMeshSceneNode(longWallMeshLeft);
	longWallNodeLeft->setMaterialFlag(video::EMF_LIGHTING, true);
	longWallNodeLeft->setPosition(core::vector3df(0, 0, 90));

	IMesh* shortWallMeshUp = smgr->getMesh("../media/ShortWall.3ds");
	IMeshSceneNode* shortWallNodeUp = smgr->addMeshSceneNode(shortWallMeshUp);
	shortWallNodeUp->setMaterialFlag(video::EMF_LIGHTING, true);
	shortWallNodeUp->setPosition(core::vector3df(78.5, 0, 0));

	IMesh* shortWallMeshDown = smgr->getMesh("../media/ShortWall.3ds");
	IMeshSceneNode* shortWallNodeDown = smgr->addMeshSceneNode(shortWallMeshDown);
	shortWallNodeDown->setMaterialFlag(video::EMF_LIGHTING, true);
	shortWallNodeDown->setPosition(core::vector3df(-93.5, 0, 0));

	ISceneNode* cube = smgr->addCubeSceneNode();
	if (cube) {
		cube->setPosition(core::vector3df(-30, 10, 10));
		cube->setMaterialTexture(0, driver->getTexture(crateDiffuse));
		cube->setMaterialTexture(1, driver->getTexture(crateNormal));
		cube->setMaterialFlag(video::EMF_LIGHTING, true);
	}

	IMesh* playerMesh = smgr->getMesh("../media/Color_Player_Large.3ds");
	if (playerMesh) {
		playerMesh->setMaterialFlag(EMF_LIGHTING, false);
	}
	IMeshSceneNode* playerObject = smgr->addMeshSceneNode(playerMesh);
	if (playerObject)
	{
		playerObject->setPosition(core::vector3df(0, 0, 30));
	}
	ISceneNode* cube2 = smgr->addCubeSceneNode();
	if (cube2) {
		cube2->setPosition(vector3df(10, 10, -30));
		cube2->setMaterialTexture(0, driver->getTexture(crateDiffuse));
		cube2->setMaterialTexture(1, driver->getTexture(crateNormal));
		cube2->setMaterialFlag(video::EMF_LIGHTING, true);
	}

	vector3df oldPosition = playerObject->getPosition();

	ICameraSceneNode* camera = smgr->addCameraSceneNode();

	if (camera) {
		camera->setPosition(cameraPosition);
		camera->setTarget(cameraTarget);
	}

	ILightSceneNode*  directionalLight = device->getSceneManager()->addLightSceneNode();
	SLight & lightData = directionalLight->getLightData();
	lightData.Type = ELT_DIRECTIONAL;
	directionalLight->setRotation(vector3df(90, 0, 0));
	device->getCursorControl()->setVisible(true);

	int lastFPS = -1;

	while (device->run())
	{

		vector3df nodePosition = playerObject->getPosition();
		if (!collision.SceneNodeWithSceneNode(playerObject, cube) && !collision.SceneNodeWithSceneNode(playerObject, cube2)
			&& !collision.SceneNodeWithSceneNode(playerObject, longWallNodeRight) && !collision.SceneNodeWithSceneNode(playerObject, longWallNodeLeft)
			&& !collision.SceneNodeWithSceneNode(playerObject, shortWallNodeUp) && !collision.SceneNodeWithSceneNode(playerObject, shortWallNodeDown))
			oldPosition = playerObject->getPosition();

		playerObject->setPosition(player->Move(nodePosition, inputReceiver));

		playerObject->setMaterialFlag(video::EMF_LIGHTING, inputReceiver.isLeftMouseButtonDown);

		if (collision.SceneNodeWithSceneNode(playerObject, cube) || collision.SceneNodeWithSceneNode(playerObject, cube2)
			|| collision.SceneNodeWithSceneNode(playerObject, longWallNodeLeft) || collision.SceneNodeWithSceneNode(playerObject, longWallNodeRight)
			|| collision.SceneNodeWithSceneNode(playerObject, shortWallNodeUp) || collision.SceneNodeWithSceneNode(playerObject, shortWallNodeDown)) {
			playerObject->setPosition(oldPosition);
		}
		driver->beginScene(true, true, SColor(255, 113, 113, 133));

		smgr->drawAll(); // draw the 3d scene
		guienv->drawAll();
		player->DrawHealthBar();

		driver->endScene();

		int fps = driver->getFPS();

		if (lastFPS != fps)
		{
			stringw tmp(L"KOMMANDOS - Irrlicht Engine [");

			tmp += driver->getName();
			tmp += L"] fps: ";
			tmp += fps;

			device->setWindowCaption(tmp.c_str());
			lastFPS = fps;
		}
	}
	device->drop();

	return 0;
}