#include <irrlicht.h>
#include "Collision.h"
#include "driverChoice.h"
#include "InputReceiver.h"
#include "LevelGeneration.h"
#include "EnemyBehaviour.h"
#include "EnemySpawner.h"
#include "Player.h"
#include "ParticleSystem.h"
#include "Gun.h"
#include "Score.h"
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
CMatrix4<float> projectionMatrix;

// Initialize the paths for the object its textures
const path crateDiffuse = "../media/crate/crate_diffuse.png";
const path crateNormal = "../media/crate/crate_normal.png";

int main()
{
	InputReceiver inputReceiver;
	// Create device
	IrrlichtDevice* device = createDevice(video::EDT_DIRECT3D9,
		dimension2d<u32>(800, 600), 16, false, false, false, &inputReceiver);

	// Create instances of classes
	Collision collision;
	Player* player = new Player(device);
	ParticleSystem particles;
	//Gun* gun;
	Score score;
	LevelGeneration levelGeneration;
	EnemySpawner* enemySpawner = new EnemySpawner(device, player);
	
	// No device found
	if (!device) {
		return 1;
	}

	inputReceiver.CheckJoystickPresent(device);

	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();

	IGUIEnvironment* guienv = device->getGUIEnvironment();

	IMesh* portalMesh = smgr->getMesh("../media/PortalRed.3ds");
	IMeshSceneNode* portalNode = smgr->addMeshSceneNode(portalMesh);
	portalNode->setPosition(vector3df(75, 0, 0));

	IMesh* planeMesh = smgr->getMesh("../media/ArenaColor.3ds");
	IMeshSceneNode* planeNode = smgr->addMeshSceneNode(planeMesh);
	planeNode->setMaterialFlag(video::EMF_LIGHTING, true);

	IMesh* longWallMeshRight = smgr->getMesh("../media/LongWall.3ds");
	IMeshSceneNode* longWallNodeRight = smgr->addMeshSceneNode(longWallMeshRight);
	longWallNodeRight->setMaterialFlag(EMF_LIGHTING, true);
	longWallNodeRight->setPosition(vector3df(0, 0, -75));

	IMesh* longWallMeshLeft = smgr->getMesh("../media/LongWall.3ds");
	IMeshSceneNode* longWallNodeLeft = smgr->addMeshSceneNode(longWallMeshLeft);
	longWallNodeLeft->setMaterialFlag(EMF_LIGHTING, true);
	longWallNodeLeft->setPosition(vector3df(0, 0, 90));

	IMesh* shortWallMeshUp = smgr->getMesh("../media/ShortWall.3ds");
	IMeshSceneNode* shortWallNodeUp = smgr->addMeshSceneNode(shortWallMeshUp);
	shortWallNodeUp->setMaterialFlag(EMF_LIGHTING, true);
	shortWallNodeUp->setPosition(vector3df(78.5, 0, 0));

	IMesh* shortWallMeshDown = smgr->getMesh("../media/ShortWall.3ds");
	IMeshSceneNode* shortWallNodeDown = smgr->addMeshSceneNode(shortWallMeshDown);
	shortWallNodeDown->setMaterialFlag(EMF_LIGHTING, true);
	shortWallNodeDown->setPosition(vector3df(-93.5, 0, 0));

	ISceneNode* cube = smgr->addCubeSceneNode();
	if (cube) {
		cube->setPosition(core::vector3df(-30, 10, 10));
		cube->setMaterialTexture(0, driver->getTexture(crateDiffuse));
		cube->setMaterialTexture(1, driver->getTexture(crateNormal));
		cube->setMaterialFlag(EMF_LIGHTING, true);
	}
	ISceneNode* cube2 = smgr->addCubeSceneNode();
	if (cube2) {
		cube2->setPosition(vector3df(10, 10, -30));
		cube2->setMaterialTexture(0, driver->getTexture(crateDiffuse));
		cube2->setMaterialTexture(1, driver->getTexture(crateNormal));
		cube2->setMaterialFlag(EMF_LIGHTING, true);
	}

	// Add to collision for enemy
	collision.AddStaticToList(cube);
	collision.AddStaticToList(cube2);
	collision.AddStaticToList(longWallNodeRight);
	collision.AddStaticToList(longWallNodeLeft);
	collision.AddStaticToList(shortWallNodeUp);
	collision.AddStaticToList(shortWallNodeDown);

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

	//Generates the level(arenas), adds 2 arena's
	levelGeneration.PlaceArenas(smgr, 2);

	int lastFPS = -1;
	// In order to do framerate independent movement, we have to know
	// how long it was since the last frame
	u32 then = device->getTimer()->getTime();

	//s32 scor = 28;
	//guienv->addStaticText(core::stringw(scor).c_str(),
	//	rect<s32>(80, 80, 200, 100), true);
	score.Scoring(device);

	while (device->run())
	{
		// Work out a frame delta time.
		const u32 now = device->getTimer()->getTime();
		const f32 frameDeltaTime = (f32)(now - then) / 1000.f; // Time in seconds
		then = now;

		player->Move(inputReceiver);
		enemySpawner->UpdateEnemies();
		player->Shoot(inputReceiver, enemySpawner);
		particles.SystemParticle(device);
		driver->beginScene(true, true, SColor(255, 113, 113, 133));
		smgr->drawAll();
		guienv->drawAll();
		player->DrawHealthBar();
		score.DisplayScore(0);
		driver->endScene();

		int fps = driver->getFPS();
		if (lastFPS != fps)
		{
			stringw tmp(L"KOMMANDOS - Irrlicht Engine");

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