#include <irrlicht.h>
#include "Game.h"
#include "driverChoice.h"
#include "Collision.h"
#include "InputReceiver.h"
#include "LevelGeneration.h"
#include "EnemyBehaviour.h"
#include "EnemySpawner.h"
#include "Player.h"
#include "ParticleSystem.h"
#include "Gun.h"
#include "Score.h"
#include "Camera.h"
#include "ObjectPlacementGeneration.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

IVideoDriver* driver;
ISceneManager* smgr;
IGUIEnvironment* guienv;
InputReceiver inputReceiver;

Player* player;
Score score;
Collision _collision;
LevelGeneration levelGeneration;
ObjectPlacementGeneration objectPlacementGen;
EnemySpawner* enemySpawner;
Camera* camera;

int lastFPS = -1;
// In order to do framerate independent movement, we have to know
// how long it was since the last frame
u32 prevFrame;

// Initialize the paths for the object its textures
const path crateDiffuse = "../media/crate/crate_diffuse.png";
const path crateNormal = "../media/crate/crate_normal.png";

Game::Game()
{
	// Create device
	device = createDevice(EDT_DIRECT3D9,
		dimension2d<u32>(800, 600), 16, false, false, false, &inputReceiver);
}

// Set instance to 0 (NULL)
Game* Game::instance = 0;

Game* Game::GetInstance() {
	if (!instance)
	{
		instance = new Game();
	}
	return instance;
}

void Game::Start()
{
	// Create instances of classes
	camera = new Camera(device);
	player = new Player(device);
	enemySpawner = new EnemySpawner(device, player);
	score.Scoring(device);

	driver = device->getVideoDriver();
	smgr = device->getSceneManager();
	guienv = device->getGUIEnvironment();

	objectPlacementGen.PlaceObjects(device);

	//Create Light
	ILightSceneNode*  directionalLight = device->getSceneManager()->addLightSceneNode();
	SLight & lightData = directionalLight->getLightData();
	lightData.Type = ELT_DIRECTIONAL;
	directionalLight->setRotation(vector3df(90, 0, 0));
	device->getCursorControl()->setVisible(true);

	//Generates the level(arenas), adds 2 arena's
	levelGeneration.PlaceArenas(smgr, 2);

	//This is used to calculate frame delta time
	prevFrame = device->getTimer()->getTime();
}

void Game::Update()
{
	// Work out a frame delta time.
	const u32 currentFrame = device->getTimer()->getTime();
	const f32 frameDeltaTime = (f32)(currentFrame - prevFrame) / 1000.f; // Time in seconds
	prevFrame = currentFrame;

	camera->CameraUpdate();
	player->Move(inputReceiver);
	enemySpawner->UpdateEnemies();
	player->Shoot(inputReceiver, enemySpawner);
}

void Game::Draw()
{
	driver->beginScene(true, true, SColor(255, 113, 113, 133));
	smgr->drawAll();
	guienv->drawAll();
	player->DrawHealthBar();
	score.Scoring(device);
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
