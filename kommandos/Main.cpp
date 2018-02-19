#include <irrlicht.h>
#include "driverChoice.h"
#include "InputReceiver.h"

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

int main()
{

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

	scene::ISceneNode * sphere = smgr->addSphereSceneNode();
	if (sphere)
	{
		sphere->setPosition(core::vector3df(0, 0, 30));
		sphere->setMaterialTexture(0, driver->getTexture("../media/wall.bmp"));
		sphere->setMaterialFlag(video::EMF_LIGHTING, false);
	}
	scene::ISceneNode* cube = smgr->addCubeSceneNode();

	if (cube)
	{
		cube->setMaterialTexture(0, driver->getTexture("../media/t351sml.jpg"));
		cube->setMaterialFlag(video::EMF_LIGHTING, false);
		scene::ISceneNodeAnimator* anim =
			smgr->createFlyCircleAnimator(core::vector3df(0, 0, 30), 20.0f);
		if (anim)
		{
			cube->addAnimator(anim);
			anim->drop();
		}
	}

	const vector3df cameraPosition = vector3df(0, 150, 0);
	const vector3df cameraTarget = vector3df(0, 0, 0);

	ICameraSceneNode* camera = smgr->addCameraSceneNode();

	if (camera) {
		camera->setPosition(cameraPosition);
		camera->setTarget(cameraTarget);
	}

	device->getCursorControl()->setVisible(false);

	gui::IGUIStaticText* diagnostics = device->getGUIEnvironment()->addStaticText(
		L"", core::rect<s32>(10, 10, 400, 20));
	diagnostics->setOverrideColor(video::SColor(255, 255, 255, 0));

	int lastFPS = -1;

	// In order to do framerate independent movement, we have to know
	// how long it was since the last frame
	u32 then = device->getTimer()->getTime();

	// This is the movemen speed in units per second.
	const f32 MOVEMENT_SPEED = 30.f;

	while (device->run())
	{
		// Work out a frame delta time.
		const u32 now = device->getTimer()->getTime();
		const f32 frameDeltaTime = (f32)(now - then) / 1000.f; // Time in seconds
		then = now;

		core::vector3df nodePosition = sphere->getPosition();

		if (inputReceiver.IsKeyDown(irr::KEY_KEY_W))
			nodePosition.X += MOVEMENT_SPEED * frameDeltaTime;
		else if (inputReceiver.IsKeyDown(irr::KEY_KEY_S))
			nodePosition.X -= MOVEMENT_SPEED * frameDeltaTime;

		if (inputReceiver.IsKeyDown(irr::KEY_KEY_A))
			nodePosition.Z += MOVEMENT_SPEED * frameDeltaTime;
		else if (inputReceiver.IsKeyDown(irr::KEY_KEY_D))
			nodePosition.Z -= MOVEMENT_SPEED * frameDeltaTime;

		sphere->setPosition(nodePosition);

		driver->beginScene(true, true, video::SColor(255, 113, 113, 133));

		smgr->drawAll(); // draw the 3d scene
		device->getGUIEnvironment()->drawAll(); // draw the gui environment (the logo)

		driver->endScene();

		int fps = driver->getFPS();

		if (lastFPS != fps)
		{
			core::stringw tmp(L"Movement Example - Irrlicht Engine [");
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
