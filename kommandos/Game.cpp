#include <irrlicht.h>
#include <irrKlang.h>
#include "Game.h"
#include "driverChoice.h"
#include "Collision.h"
#include "InputReceiver.h"
#include "EnemySpawner.h"
#include "ParticleSystem.h"
#include "Score.h"
#include "UISystem.h"
#include "Camera.h"
#include "ObjectPlacementGeneration.h"
#include "SoundManager.h"
#include "Tutorial.h"
#include "PowerupSpawner.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;
using namespace irrklang;

IVideoDriver* driver;
ISceneManager* smgr;
IGUIEnvironment* guienv;
InputReceiver inputReceiver;

UISystem* uISystem;
Score score;
Collision _collision;
EnemySpawner* enemySpawner;
Camera* camera;
Collision collisionManager;
Tutorial tutorial;
PowerUpSpawner* powerSpawn;

int lastFPS = -1;
// In order to do framerate independent movement, we have to know
// how long it was since the last frame
u32 prevFrame;

// Initialize the paths for the object its textures
const path crateDiffuse = "../media/crate/crate_diffuse.png";
const path crateNormal = "../media/crate/crate_normal.png";

bool isGameOver;

Game::Game()
{
	// Create device
	device = createDevice(EDT_DIRECT3D9,
		dimension2d<u32>(800, 600), 16, false, false, true, &inputReceiver);
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

bool Game::GetIsGameOver() {
	return isGameOver;
}

bool Game::SetIsGameOver(bool state)
{
	tutorial.RemoveTutorial();
	camera->state = camera->gameover;
	return isGameOver = state;
}

void Game::Start()

{
	//Get the sound engine
	SoundManager* soundManager = soundManager->GetInstance();
	//Start Game Background song	
	soundManager->PlaySound("../media/Sounds/bgsound.mp3", true);

	// Create instances of classes
	camera = camera->GetInstance(device);
	player = new Player(device);
	enemySpawner = new EnemySpawner(device, player);
	uISystem = new UISystem(device);

	//score.Scoring is unneeded duplicate code
	score.Scoring(device);
	tutorial.ShowTutorial(device);

	driver = device->getVideoDriver();
	smgr = device->getSceneManager();
	guienv = device->getGUIEnvironment();

	objectPlacementGen.PlaceObjects(device);
	powerSpawn->PowerUpDevice(device);

	uISystem->InitUISystem(device);

	//Create Light
	ILightSceneNode*  directionalLight = device->getSceneManager()->addLightSceneNode();
	SLight & lightData = directionalLight->getLightData();
	lightData.Type = ELT_DIRECTIONAL;
	directionalLight->setRotation(vector3df(90, 0, 0));
	device->getCursorControl()->setVisible(true);

	//This is used to calculate frame delta time
	prevFrame = device->getTimer()->getTime();
}

void Game::Update()
{
	// Work out a frame delta time.
	const u32 currentFrame = device->getTimer()->getTime();
	const f32 frameDeltaTime = (f32)(currentFrame - prevFrame) / 1000.f; // Time in seconds
	prevFrame = currentFrame;
	camera->CameraUpdate(frameDeltaTime);
	if (tutorial.isTutorialActive) {
		tutorial.Update(frameDeltaTime);
	}
	if (!isGameOver )
	{
		player->Move(inputReceiver);
		player->Shoot(inputReceiver, enemySpawner);
		if(!tutorial.isTutorialActive)
			enemySpawner->UpdateEnemies();
		collisionManager.DiscreteCollisionUpdate(frameDeltaTime);
	}
}

void Game::Draw()
{
	driver->beginScene(true, true, SColor(255, 113, 113, 133));
	smgr->drawAll();
	guienv->drawAll();
	player->DrawHealthBar();
	score.Scoring(device);
	uISystem->WaveUI(device);
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

Player* Game::GetPlayer()
{
	return player;
}