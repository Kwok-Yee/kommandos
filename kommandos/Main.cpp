#include <irrlicht.h>
#include "Collision.h"
#include "driverChoice.h"
#include "InputReceiver.h"
#include "EnemyBehaviour.h"
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

//ProjectionMatrix for the orthographic camera
irr::core::CMatrix4<float> projectionMatrix;

// Initialize the paths for the object its textures
const path crateDiffuse = "../media/crate/crate_diffuse.png";
const path crateNormal = "../media/crate/crate_normal.png";

int main()
{
	// Create instances of classes
	InputReceiver inputReceiver;
	EnemyBehaviour enemyController;
	Collision collision;

	// Create device
	IrrlichtDevice* device = createDevice(video::EDT_DIRECT3D9,
		dimension2d<u32>(800, 600), 16, false, false, false, &inputReceiver);

	// No device found
	if (!device) {
		return 1;
	}
	Player* player = new Player(device);
	vector3df playerRotation;

	inputReceiver.CheckJoystickPresent(device);

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
	ISceneNode* cube2 = smgr->addCubeSceneNode();
	if (cube2) {
		cube2->setPosition(vector3df(10, 10, -30));
		cube2->setMaterialTexture(0, driver->getTexture(crateDiffuse));
		cube2->setMaterialTexture(1, driver->getTexture(crateNormal));
		cube2->setMaterialFlag(video::EMF_LIGHTING, true);
	}

	// Add to collision for enemy
	collision.AddStaticToList(cube);
	collision.AddStaticToList(cube2);

	IMesh* playerMesh = smgr->getMesh("../media/PlayerModel.3ds");
	
	if (playerMesh) {
		playerMesh->setMaterialFlag(EMF_LIGHTING, false);
	}
	IMeshSceneNode* playerObject = smgr->addMeshSceneNode(playerMesh);
	if (playerObject)
	{
		playerObject->setPosition(core::vector3df(0, 0, 30));
	}
	vector3df oldPosition = playerObject->getPosition();

	irr::core::array<IMeshSceneNode*> enemies;
	int enemiesToSpawn = 2;
	int positionMultiplier = 10;
	for (int i = 0; i < enemiesToSpawn; i++)
		enemies.push_back(enemyController.Spawn(device, vector3df((i + 1)*positionMultiplier, 0, (i + 1)*positionMultiplier)));

	const vector3df cameraPosition = vector3df(0, 150, 0);

	ICameraSceneNode* camera = smgr->addCameraSceneNode();

	if (camera) {
		camera->setPosition(cameraPosition);
		camera->setTarget(cameraTarget);
		projectionMatrix.buildProjectionMatrixOrthoLH(f32(100 * 2), f32(60 * 2 * 1080 / 720), 1, 300);
		camera->setProjectionMatrix(projectionMatrix, true);
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
		core::vector3df nodePosition = playerObject->getPosition();

		if (inputReceiver.GetJoystickInfo().size() > 0)
		{
			joystickConnected = true;

			f32 hMove = 0.f;
			f32 vMove = 0.f;
			f32 moveSpeed = 50.f;
			// Store a local reference from joystickState
			const SEvent::SJoystickEvent & joystickData = inputReceiver.joystickState;

			// We receive the full analog range of the axes, and so have to implement our
			// own dead zone.  This is an empirical value, since some joysticks have more
			// jitter or creep around the center point than others.  We'll use 5% of the
			// range as the dead zone, but generally you would want to give the user the
			// option to change this.
			const f32 DEAD_ZONE = 0.05f;

			vMove =
				(f32)joystickData.Axis[SEvent::SJoystickEvent::AXIS_X] / -32767.f;
			if (fabs(vMove) < DEAD_ZONE)
				vMove = 0.f;

			hMove =
				(f32)joystickData.Axis[SEvent::SJoystickEvent::AXIS_Y] / -32767.f;
			if (fabs(hMove) < DEAD_ZONE)
				hMove = 0.f;

			if (!core::equals(vMove, 0.f) || !core::equals(hMove, 0.f))
			{
				nodePosition.X += moveSpeed * frameDeltaTime * hMove;
				nodePosition.Z += moveSpeed * frameDeltaTime * vMove;
			}
			playerObject->setMaterialFlag(video::EMF_LIGHTING, joystickData.IsButtonPressed(7));
		}


		if (!collision.SceneNodeWithSceneNode(playerObject, cube) && !collision.SceneNodeWithSceneNode(playerObject, cube2)
			&& !collision.SceneNodeWithSceneNode(playerObject, longWallNodeRight) && !collision.SceneNodeWithSceneNode(playerObject, longWallNodeLeft)
			&& !collision.SceneNodeWithSceneNode(playerObject, shortWallNodeUp) && !collision.SceneNodeWithSceneNode(playerObject, shortWallNodeDown))
			oldPosition = playerObject->getPosition();
		if (!joystickConnected) {
			playerObject->setPosition(player->Move(nodePosition, inputReceiver));
			playerObject->setRotation(player->playerRotate(playerRotation, inputReceiver));
			playerObject->setMaterialFlag(video::EMF_LIGHTING, inputReceiver.isLeftMouseButtonDown);
		}
		playerObject->setPosition(nodePosition);
		if (collision.SceneNodeWithSceneNode(playerObject, cube) || collision.SceneNodeWithSceneNode(playerObject, cube2)
			|| collision.SceneNodeWithSceneNode(playerObject, longWallNodeLeft) || collision.SceneNodeWithSceneNode(playerObject, longWallNodeRight)
			|| collision.SceneNodeWithSceneNode(playerObject, shortWallNodeUp) || collision.SceneNodeWithSceneNode(playerObject, shortWallNodeDown)) {
			playerObject->setPosition(oldPosition);
		}

		// Update all enemies
		for (int i = 0; i < enemies.size(); i++) 
		{
			if (enemyController.Update(enemies[i], nodePosition, frameDeltaTime) && player->health > 0)
				player->TakeDamage(100);
		}

		driver->beginScene(true, true, SColor(255, 113, 113, 133));
		smgr->drawAll();
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