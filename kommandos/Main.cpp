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
#pragma comment(linker, "/subsystem:console /ENTRY:mainCRTStartup")
#endif

// This is the movement speed in units per second.
const f32 MOVEMENT_SPEED = 50.f;

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

	core::array<SJoystickInfo> joystickInfo;
	if (device->activateJoysticks(joystickInfo))
	{
		std::cout << "Joystick support is enabled and " << joystickInfo.size() << " joystick(s) are present." << std::endl;

		for (u32 joystick = 0; joystick < joystickInfo.size(); ++joystick)
		{
			std::cout << "Joystick " << joystick << ":" << std::endl;
			std::cout << "\tName: '" << joystickInfo[joystick].Name.c_str() << "'" << std::endl;
			std::cout << "\tAxes: " << joystickInfo[joystick].Axes << std::endl;
			std::cout << "\tButtons: " << joystickInfo[joystick].Buttons << std::endl;

			std::cout << "\tHat is: ";

			switch (joystickInfo[joystick].PovHat)
			{
			case SJoystickInfo::POV_HAT_PRESENT:
				std::cout << "present" << std::endl;
				break;

			case SJoystickInfo::POV_HAT_ABSENT:
				std::cout << "absent" << std::endl;
				break;

			case SJoystickInfo::POV_HAT_UNKNOWN:
			default:
				std::cout << "unknown" << std::endl;
				break;
			}
		}
	}
	else
	{
		std::cout << "Joystick support is not enabled." << std::endl;
	}

	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();
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
	IMeshSceneNode* player = smgr->addMeshSceneNode(playerMesh);
	if (player)
	{
		player->setPosition(core::vector3df(0, 0, 30));
	}
	ISceneNode* cube2 = smgr->addCubeSceneNode();
	if (cube2) {
		cube2->setPosition(vector3df(10, 10, -30));
		cube2->setMaterialTexture(0, driver->getTexture(crateDiffuse));
		cube2->setMaterialTexture(1, driver->getTexture(crateNormal));
		cube2->setMaterialFlag(video::EMF_LIGHTING, true);
	}

	core::vector3df oldPosition = player->getPosition();

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

	while (device->run())
	{
		// Work out a frame delta time.
		const u32 now = device->getTimer()->getTime();
		const f32 frameDeltaTime = (f32)(now - then) / 1000.f; // Time in seconds
		then = now;

		bool joystickConnected = false;
		core::vector3df nodePosition = player->getPosition();

		if (joystickInfo.size() > 0)
		{
			joystickConnected = true;
			f32 hMove = 0.f;
			f32 vMove = 0.f;

			// Store a local reference from joystickState
			const SEvent::SJoystickEvent & joystickData = inputReceiver.joystickState;

			// We receive the full analog range of the axes, and so have to implement our
			// own dead zone.  This is an empirical value, since some joysticks have more
			// jitter or creep around the center point than others.  We'll use 5% of the
			// range as the dead zone, but generally you would want to give the user the
			// option to change this.
			const f32 DEAD_ZONE = 0.05f;

			hMove =
				(f32)joystickData.Axis[SEvent::SJoystickEvent::AXIS_Y] / -32767.f;
			if (fabs(hMove) < DEAD_ZONE)
				hMove = 0.f;

			vMove =
				(f32)joystickData.Axis[SEvent::SJoystickEvent::AXIS_X] / 32767.f;
			if (fabs(vMove) < DEAD_ZONE)
				vMove = 0.f;

			// POV hat info is only currently supported on Windows, but the value is
			// guaranteed to be 65535 if it's not supported, so we can check its range.
			const u16 povDegrees = joystickData.POV / 100;
			if (povDegrees < 360)
			{
				if (povDegrees > 0 && povDegrees < 180)
					vMove = 1.f;
				else if (povDegrees > 180)
					vMove = -1.f;

				if (povDegrees > 90 && povDegrees < 270)
					hMove = -1.f;
				else if (povDegrees > 270 || povDegrees < 90)
					hMove = +1.f;
			}

			if (!core::equals(vMove, 0.f) || !core::equals(hMove, 0.f))
			{
				nodePosition.X += MOVEMENT_SPEED * frameDeltaTime * hMove;
				nodePosition.Z += -(MOVEMENT_SPEED * frameDeltaTime * vMove);
			}
			player->setMaterialFlag(video::EMF_LIGHTING, joystickData.IsButtonPressed(7));
		}

		if (!collision.SceneNodeWithSceneNode(player, cube) && !collision.SceneNodeWithSceneNode(player, cube2)
			&& !collision.SceneNodeWithSceneNode(player, longWallNodeRight) && !collision.SceneNodeWithSceneNode(player, longWallNodeLeft)
			&& !collision.SceneNodeWithSceneNode(player, shortWallNodeUp) && !collision.SceneNodeWithSceneNode(player, shortWallNodeDown))
			oldPosition = player->getPosition();

		if (!joystickConnected) {
			if (inputReceiver.IsKeyDown(irr::KEY_KEY_W))
				nodePosition.X += MOVEMENT_SPEED * frameDeltaTime;
			else if (inputReceiver.IsKeyDown(irr::KEY_KEY_S))
				nodePosition.X -= MOVEMENT_SPEED * frameDeltaTime;

			if (inputReceiver.IsKeyDown(irr::KEY_KEY_A))
				nodePosition.Z += MOVEMENT_SPEED * frameDeltaTime;
			else if (inputReceiver.IsKeyDown(irr::KEY_KEY_D))
				nodePosition.Z -= MOVEMENT_SPEED * frameDeltaTime;

			player->setMaterialFlag(video::EMF_LIGHTING, inputReceiver.isLeftMouseButtonDown);
		}

		player->setPosition(nodePosition);


		if (collision.SceneNodeWithSceneNode(player, cube) || collision.SceneNodeWithSceneNode(player, cube2)
			|| collision.SceneNodeWithSceneNode(player, longWallNodeLeft) || collision.SceneNodeWithSceneNode(player, longWallNodeRight)
			|| collision.SceneNodeWithSceneNode(player, shortWallNodeUp) || collision.SceneNodeWithSceneNode(player, shortWallNodeDown)) {
			player->setPosition(oldPosition);
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