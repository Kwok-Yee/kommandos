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
		core::dimension2d<u32>(800, 600), 16, false, false, false, &inputReceiver);

	// No device found
	if (!device) {
		return 1;
	}

	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();
	IGUIEnvironment* guienv = device->getGUIEnvironment();


	IMesh* planeMesh = smgr->getMesh("../media/Arena.3ds");
	IMeshSceneNode* planeNode = smgr->addMeshSceneNode(planeMesh);
	planeNode->setMaterialFlag(video::EMF_LIGHTING, true);

	IMesh* LongWallMesh = smgr->getMesh("../media/LongWall.3ds");
	IMeshSceneNode* LongWallNode = smgr->addMeshSceneNode(LongWallMesh);
	LongWallNode->setMaterialFlag(video::EMF_LIGHTING, true);

	IMesh* LongWallMesh2 = smgr->getMesh("../media/LongWall.3ds");
	IMeshSceneNode* LongWallNode2 = smgr->addMeshSceneNode(LongWallMesh2);
	LongWallNode2->setMaterialFlag(video::EMF_LIGHTING, true);
	LongWallNode2->setPosition(core::vector3df(0, 0, 15));
	
	IMesh* ShortWallMesh = smgr->getMesh("../media/ShortWall.3ds");
	IMeshSceneNode* ShortWallNode = smgr->addMeshSceneNode(ShortWallMesh);
	ShortWallNode->setMaterialFlag(video::EMF_LIGHTING, true);

	IMesh* ShortWallMesh2 = smgr->getMesh("../media/ShortWall.3ds");
	IMeshSceneNode* ShortWallNode2 = smgr->addMeshSceneNode(ShortWallMesh2);
	ShortWallNode2->setMaterialFlag(video::EMF_LIGHTING, true);
	ShortWallNode2->setPosition(core::vector3df(-15, 0, 0));

	ISceneNode* cube = smgr->addCubeSceneNode();
	if (cube) {
		cube->setPosition(core::vector3df(-30, 0, 10));
		cube->setMaterialTexture(0, driver->getTexture(crateDiffuse));
		cube->setMaterialTexture(1, driver->getTexture(crateNormal));
		cube->setMaterialFlag(video::EMF_LIGHTING, true);
	}

	ISceneNode* cube2 = smgr->addCubeSceneNode();
	if (cube2) {
		cube2->setPosition(vector3df(10, 0, -30));
		cube2->setMaterialTexture(0, driver->getTexture(crateDiffuse));
		cube2->setMaterialTexture(1, driver->getTexture(crateNormal));
		cube2->setMaterialFlag(video::EMF_LIGHTING, true);
	}

	ISceneNode * sphere = smgr->addSphereSceneNode();

	if (sphere)
	{
		sphere->setPosition(core::vector3df(0, 0, 0));
		sphere->setMaterialTexture(0, driver->getTexture("../media/wall.bmp"));
		sphere->setMaterialFlag(video::EMF_LIGHTING, true);
	}
	core::vector3df oldPosition = sphere->getPosition();

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

		core::vector3df nodePosition = sphere->getPosition();
		if (!collision.SceneNodeWithSceneNode(sphere, LongWallNode2) && !collision.SceneNodeWithSceneNode(sphere, cube2))
			oldPosition = sphere->getPosition();

		if (inputReceiver.IsKeyDown(irr::KEY_KEY_W))
			nodePosition.X += MOVEMENT_SPEED * frameDeltaTime;
		else if (inputReceiver.IsKeyDown(irr::KEY_KEY_S))
			nodePosition.X -= MOVEMENT_SPEED * frameDeltaTime;

		if (inputReceiver.IsKeyDown(irr::KEY_KEY_A))
			nodePosition.Z += MOVEMENT_SPEED * frameDeltaTime;
		else if (inputReceiver.IsKeyDown(irr::KEY_KEY_D))
			nodePosition.Z -= MOVEMENT_SPEED * frameDeltaTime;

		sphere->setPosition(nodePosition);

		sphere->setMaterialFlag(video::EMF_LIGHTING, inputReceiver.isLeftMouseButtonDown);

		if (collision.SceneNodeWithSceneNode(sphere, LongWallNode2) || collision.SceneNodeWithSceneNode(sphere, cube2)) {
			sphere->setPosition(oldPosition);
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