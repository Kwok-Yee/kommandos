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
ParticleSystem particles;
Score score;
LevelGeneration levelGeneration;
ObjectPlacementGeneration objectPlacementGen;
EnemySpawner* enemySpawner;

const vector3df cameraPosition = vector3df(0, 120, 0);
const vector3df cameraTarget = vector3df(0, 0, 0);

int lastFPS = -1;
// In order to do framerate independent movement, we have to know
// how long it was since the last frame
u32 prevFrame;

//ProjectionMatrix for the orthographic camera
CMatrix4<float> projectionMatrix;

// Initialize the paths for the object its textures
const path crateDiffuse = "../media/crate/crate_diffuse.png";
const path crateNormal = "../media/crate/crate_normal.png";

Game::Game()
{
	// Create device
	device = createDevice(EDT_DIRECT3D9,
		dimension2d<u32>(800, 600), 16, false, false, false, &inputReceiver);
}

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
	player = new Player(device);
	enemySpawner = new EnemySpawner(device, player);
	score.Scoring(device);
	particles.SystemParticle(device);

	driver = device->getVideoDriver();
	smgr = device->getSceneManager();
	guienv = device->getGUIEnvironment();

	objectPlacementGen.PlaceObjects(device);

	//Create Camera
	const vector3df cameraPosition = vector3df(0, 150, 0);
	ICameraSceneNode* camera = smgr->addCameraSceneNode();
	if (camera) {
		camera->setPosition(cameraPosition);
		camera->setTarget(cameraTarget);
		projectionMatrix.buildProjectionMatrixOrthoLH(f32(100 * 2), f32(60 * 2 * 1080 / 720), 1, 300);
		camera->setProjectionMatrix(projectionMatrix, true);
	}

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
