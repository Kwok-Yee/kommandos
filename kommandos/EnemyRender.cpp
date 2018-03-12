#include <irrlicht.h>
#include "driverChoice.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif	

// This is the movement speed in units per second.
const f32 ENEMY_MOVEMENT_SPEED = 15.f;
const f32 PLAYER_MOVEMENT_SPEED = 50.f;
u32 now;
f32 frameDeltaTime;

class MyEventReceiver : public IEventReceiver
{

public:
	// This is the one method that we have to implement
	virtual bool OnEvent(const SEvent& event)
	{
		// Remember whether each key is down or up
		if (event.EventType == irr::EET_KEY_INPUT_EVENT)
			KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;

		return false;
	}

	// This is used to check whether a key is being held down
	virtual bool IsKeyDown(EKEY_CODE keyCode) const
	{
		return KeyIsDown[keyCode];
	}

	MyEventReceiver()
	{
		for (u32 i = 0; i < KEY_KEY_CODES_COUNT; ++i)
			KeyIsDown[i] = false;
	}

private:
	// We use this array to store the current state of each key
	bool KeyIsDown[KEY_KEY_CODES_COUNT];
};

class Movement
{
public:
	virtual void MoveObjects(scene::ISceneNode* playerNode, scene::IMeshSceneNode* enemyNode, MyEventReceiver receiver) {
		EnemyMoveToPlayer(playerNode, enemyNode);
		MovePlayer(playerNode, receiver);
	}

private:
	void EnemyMoveToPlayer(scene::ISceneNode* playerNode, scene::IMeshSceneNode* enemyNode)
	{
		vector3df enemyPosition = enemyNode->getPosition();
		vector3df playerPosition = playerNode->getPosition();
		
		vector3df delta = playerPosition - enemyPosition; //save delta
		vector3df deltaNormalized = delta;
		deltaNormalized.normalize(); //delta gets normalized otherwise

		if (delta.getLength() > vector3df(1, 1, 1).getLength())
		{
			enemyPosition += deltaNormalized * ENEMY_MOVEMENT_SPEED * frameDeltaTime;
		}

		enemyNode->setRotation(core::vector3df(0, atan2(deltaNormalized.X, deltaNormalized.Z) * 180 / PI, 0));
		enemyNode->setPosition(enemyPosition);
	}

	void MovePlayer(scene::ISceneNode* playerNode, MyEventReceiver &receiver) {
		vector3df playerPosition = playerNode->getPosition();

		if (receiver.IsKeyDown(irr::KEY_KEY_W)) { // up
			playerPosition.Z += PLAYER_MOVEMENT_SPEED * frameDeltaTime;
			playerNode->setRotation(core::vector3df(0, 0, 0));
		}
		else if (receiver.IsKeyDown(irr::KEY_KEY_S)) { // down
			playerNode->setRotation(core::vector3df(0, 180, 0));
			playerPosition.Z -= PLAYER_MOVEMENT_SPEED * frameDeltaTime;
		}
		if (receiver.IsKeyDown(irr::KEY_KEY_A)) { // left
			playerPosition.X -= PLAYER_MOVEMENT_SPEED * frameDeltaTime;
			playerNode->setRotation(core::vector3df(0, -90, 0));
		}
		else if (receiver.IsKeyDown(irr::KEY_KEY_D)) { // right
			playerPosition.X += PLAYER_MOVEMENT_SPEED * frameDeltaTime;
			playerNode->setRotation(core::vector3df(0, 90, 0));
		}

		playerNode->setPosition(playerPosition);
	}
};

int main()
{
	// ask user for driver
	video::E_DRIVER_TYPE driverType = video::EDT_DIRECT3D9;

	// create device
	MyEventReceiver receiver;
	Movement movement;

	IrrlichtDevice* device = createDevice(driverType,
		core::dimension2d<u32>(640, 480), 16, false, false, false, &receiver);

	if (device == 0)
		return 1; // could not create selected driver.

	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();

	// add sphere (PLAYER) 
#pragma region SpawnPlayer
	scene::ISceneNode* node = smgr->addSphereSceneNode();
	if (node)
	{
		node->setPosition(core::vector3df(0, 0, 30));
		node->setMaterialTexture(0, driver->getTexture("../media/wall.bmp"));
		node->setMaterialFlag(video::EMF_LIGHTING, false);
	}
#pragma endregion SpawnPlayer
	// spawn enemy
#pragma region SpawnEnemy
	scene::IMeshSceneNode* anms =
		smgr->addMeshSceneNode(smgr->getMesh("../media/ninja.b3d"));
	if (anms)
	{
		anms->setMaterialFlag(video::EMF_LIGHTING, false);
		anms->setScale(vector3df(10,10,10));
		/*anms->setFrameLoop(0, 13);
		anms->setAnimationSpeed(15);*/
		//anms->setMD2Animation(scene::EMAT_RUN);

		anms->setScale(core::vector3df(2.f, 2.f, 2.f));
		anms->setRotation(core::vector3df(0, -90, 0));
		anms->setMaterialTexture(0, driver->getTexture("../media/nskinrd.jpg"));
	}

#pragma endregion SpawnEnemy

	smgr->addCameraSceneNodeFPS();
	device->getCursorControl()->setVisible(false);

	int lastFPS = -1;

	// In order to do framerate independent movement, we have to know
	// how long it was since the last frame
	u32 then = device->getTimer()->getTime();


	while (device->run())
	{
		// Work out a frame delta time.
		now = device->getTimer()->getTime();
		frameDeltaTime = (f32)(now - then) / 1000.f; // Time in seconds
		then = now;

		movement.MoveObjects(node, anms, receiver);

		driver->beginScene(true, true, video::SColor(255, 113, 113, 133));

		smgr->drawAll(); // draw the 3d scene

		driver->endScene();

		int fps = driver->getFPS();

		if (lastFPS != fps)
		{
			core::stringw tmp(L"Movement - Irrlicht Engine [");
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
