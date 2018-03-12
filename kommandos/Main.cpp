#include <irrlicht.h>
#include "Collision.h"
#include "driverChoice.h"
#include "InputReceiver.h"
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
//#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif	

const vector3df cameraPosition = vector3df(0, 50, 0);
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
		core::dimension2d<u32>(800, 600), 16, false, false, false, &inputReceiver);

	// No device found
	if (!device) {
		return 1;
	}

	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();
	IGUIEnvironment* guienv = device->getGUIEnvironment();

	ISceneNode* cube = smgr->addCubeSceneNode();

	if (cube) {
		cube->setPosition(core::vector3df(50, 0, 0));
		cube->setMaterialTexture(0, driver->getTexture(crateDiffuse));
		cube->setMaterialTexture(1, driver->getTexture(crateNormal));
		cube->setMaterialFlag(video::EMF_LIGHTING, false); // Set to false because of no lighting
	}

	IMesh* playerMesh = smgr->getMesh("../media/Colored_Player_Sized.3ds");
	if (playerMesh) {
		playerMesh->setMaterialFlag(EMF_LIGHTING, false);
	}
	IMeshSceneNode* playerNode = smgr->addMeshSceneNode(playerMesh);
	if (playerNode)
	{
		playerNode->setPosition(core::vector3df(0, 0, 30));
	}
	ISceneNode* cube2 = smgr->addCubeSceneNode();
	if (cube2) {
		cube2->setPosition(vector3df(10, 0, -30));
		cube2->setMaterialTexture(0, driver->getTexture(crateDiffuse));
		cube2->setMaterialTexture(1, driver->getTexture(crateNormal));
		cube2->setMaterialFlag(video::EMF_LIGHTING, false); // Set to false because of no lighting
	}

	core::vector3df oldPosition = playerNode->getPosition();

	ICameraSceneNode* camera = smgr->addCameraSceneNode();

	if (camera) {
		camera->setPosition(cameraPosition);
		camera->setTarget(cameraTarget);
	}

	device->getCursorControl()->setVisible(true);

	int lastFPS = -1;

	// In order to do framerate independent movement, we have to know
	// how long it was since the last frame
	u32 then = device->getTimer()->getTime();

	// This is the movement speed in units per second.
	const f32 MOVEMENT_SPEED = 30.f;

	while (device->run())
	{
		// Work out a frame delta time.
		const u32 now = device->getTimer()->getTime();
		const f32 frameDeltaTime = (f32)(now - then) / 1000.f; // Time in seconds
		then = now;

		core::vector3df nodePosition = playerNode->getPosition();
		if (!collision.SceneNodeWithSceneNode(playerNode, cube) && !collision.SceneNodeWithSceneNode(playerNode, cube2))
			oldPosition = playerNode->getPosition();

		if (inputReceiver.IsKeyDown(irr::KEY_KEY_W))
			nodePosition.X += MOVEMENT_SPEED * frameDeltaTime;
		else if (inputReceiver.IsKeyDown(irr::KEY_KEY_S))
			nodePosition.X -= MOVEMENT_SPEED * frameDeltaTime;

		if (inputReceiver.IsKeyDown(irr::KEY_KEY_A))
			nodePosition.Z += MOVEMENT_SPEED * frameDeltaTime;
		else if (inputReceiver.IsKeyDown(irr::KEY_KEY_D))
			nodePosition.Z -= MOVEMENT_SPEED * frameDeltaTime;

		playerNode->setPosition(nodePosition);

		playerNode->setMaterialFlag(video::EMF_LIGHTING, inputReceiver.isLeftMouseButtonDown);

		if (collision.SceneNodeWithSceneNode(playerNode, cube) || collision.SceneNodeWithSceneNode(playerNode, cube2)) {
			playerNode->setPosition(oldPosition);
		}

		driver->beginScene(true, true, video::SColor(255, 113, 113, 133));

		smgr->drawAll(); // draw the 3d scene
		guienv->drawAll();

		driver->endScene();

		int fps = driver->getFPS();

		if (lastFPS != fps)
		{
			core::stringw tmp(L"KOMMANDOS - Irrlicht Engine [");

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
