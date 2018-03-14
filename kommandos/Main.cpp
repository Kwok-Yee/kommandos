#include <irrlicht.h>
#include "Collision.h"
#include "driverChoice.h"
#include "InputReceiver.h"
#include "EnemyBehaviour.h"
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

int main()
{
	// Create instances of classes
	InputReceiver inputReceiver;
	EnemyBehaviour enemyController;
	Collision collision;

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

	IMeshSceneNode* Cube1 = smgr->addCubeSceneNode();
	collision.AddStaticToList(Cube1);
	IMeshSceneNode* Cube2 = smgr->addCubeSceneNode();
	Cube2->setPosition(vector3df(10, 0, -30));
	collision.AddStaticToList(Cube2);

	ISceneNode * sphere = smgr->addSphereSceneNode();
	if (sphere)
	{
		sphere->setPosition(core::vector3df(0, 0, 30));
		sphere->setMaterialTexture(0, driver->getTexture("../media/wall.bmp"));
		sphere->setMaterialFlag(video::EMF_LIGHTING, false);
	}
	core::vector3df oldPosition = sphere->getPosition();

	//spawn enemies
	irr::core::array<IMeshSceneNode*> enemies;
	int enemiesToSpawn = 2;
	int positionMultiplier = 10;
	for (int i = 0; i < enemiesToSpawn; i++)
		enemies.push_back(enemyController.Spawn(device, vector3df((i + 1)*positionMultiplier, 0, (i + 1)*positionMultiplier)));

	const vector3df cameraPosition = vector3df(0, 50, 0);
	const vector3df cameraTarget = vector3df(0, 0, 0);

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

#pragma region Movement
		core::vector3df nodePosition = sphere->getPosition();
		if (!collision.SceneNodeWithSceneNode(sphere, Cube1) && !collision.SceneNodeWithSceneNode(sphere, Cube2))
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

		if (collision.SceneNodeWithSceneNode(sphere, Cube1) || collision.SceneNodeWithSceneNode(sphere, Cube2)) {
			sphere->setPosition(oldPosition);
		}

		sphere->setMaterialFlag(video::EMF_LIGHTING, inputReceiver.isLeftMouseButtonDown);
		driver->beginScene(true, true, video::SColor(255, 113, 113, 133));

#pragma endregion Movement

		// Update all enemies
		for (int i = 0; i < enemies.size(); i++)
			enemyController.Update(enemies[i], nodePosition, frameDeltaTime);

		smgr->drawAll();
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
