#include <irrlicht.h>

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

vector3df cameraPosition = vector3df(0, 50, 0);
vector3df cameraTarget = vector3df(0, 0, 0);
vector3df previousNodePosition;

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
		for (u32 i = 0; i<KEY_KEY_CODES_COUNT; ++i)
			KeyIsDown[i] = false;
	}

private:
	// We use this array to store the current state of each key
	bool KeyIsDown[KEY_KEY_CODES_COUNT];
};

int main()
{

	// ask user for driver
	video::E_DRIVER_TYPE driverType = video::EDT_DIRECT3D9;
	if (driverType == video::EDT_COUNT)
		return 1;

	// create device
	MyEventReceiver receiver;

	IrrlichtDevice* device = createDevice(driverType,
		core::dimension2d<u32>(800, 600), 16, false, false, false, &receiver);

	if (device == 0)
		return 1; // could not create selected driver.

	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();
	IGUIEnvironment* guienv = device->getGUIEnvironment();


	IMesh* planeMesh = smgr->getMesh("../media/Arena.3ds");
	IMeshSceneNode* planeNode = smgr->addMeshSceneNode(planeMesh);
	planeNode->setMaterialFlag(video::EMF_LIGHTING, false);

	IMesh* LongWallMesh = smgr->getMesh("../media/LongWall.3ds");
	IMeshSceneNode* LongWallNode = smgr->addMeshSceneNode(LongWallMesh);
	LongWallNode->setMaterialFlag(video::EMF_LIGHTING, false);

	IMesh* LongWallMesh2 = smgr->getMesh("../media/LongWall.3ds");
	IMeshSceneNode* LongWallNode2 = smgr->addMeshSceneNode(LongWallMesh2);
	LongWallNode2->setMaterialFlag(video::EMF_LIGHTING, false);
	LongWallNode2->setPosition(core::vector3df(0, 0, 15));
	
	IMesh* ShortWallMesh = smgr->getMesh("../media/ShortWall.3ds");
	IMeshSceneNode* ShortWallNode = smgr->addMeshSceneNode(ShortWallMesh);
	ShortWallNode->setMaterialFlag(video::EMF_LIGHTING, false);

	IMesh* ShortWallMesh2 = smgr->getMesh("../media/ShortWall.3ds");
	IMeshSceneNode* ShortWallNode2 = smgr->addMeshSceneNode(ShortWallMesh2);
	ShortWallNode2->setMaterialFlag(video::EMF_LIGHTING, false);
	ShortWallNode2->setPosition(core::vector3df(-15, 0, 0));


	scene::ISceneNode * node = smgr->addSphereSceneNode();
	if (node)
	{
		node->setPosition(core::vector3df(0, 0, 2));
		node->setMaterialTexture(0, driver->getTexture("../../media/wall.bmp"));
		node->setMaterialFlag(video::EMF_LIGHTING, false);
	}


	u32 then = device->getTimer()->getTime();
	const f32 MOVEMENT_SPEED = 5.f;

	ICameraSceneNode* camera = smgr->addCameraSceneNode();

	if (camera)
	{
		camera->setPosition(cameraPosition);
		camera->setTarget(cameraTarget);
	}

	while (device->run())
	{
		// Work out a frame delta time.
		const u32 now = device->getTimer()->getTime();
		const f32 frameDeltaTime = (f32)(now - then) / 1000.f; // Time in seconds
		then = now;

		core::vector3df nodePosition = node->getPosition();
		core::vector3df planePosition = planeNode->getPosition();

		if (receiver.IsKeyDown(irr::KEY_KEY_W))//up
			nodePosition.Z += MOVEMENT_SPEED * frameDeltaTime;
		else if (receiver.IsKeyDown(irr::KEY_KEY_S))//down
			nodePosition.Z -= MOVEMENT_SPEED * frameDeltaTime;

			if (receiver.IsKeyDown(irr::KEY_KEY_A))//left
				nodePosition.X -= MOVEMENT_SPEED * frameDeltaTime;
			else if (receiver.IsKeyDown(irr::KEY_KEY_D))//right
				nodePosition.X += MOVEMENT_SPEED * frameDeltaTime;

		node->setPosition(nodePosition);

		previousNodePosition = nodePosition;

		driver->beginScene(true, true, SColor(255, 100, 101, 140));

		smgr->drawAll();
		guienv->drawAll();

		driver->endScene();
	}
	device->drop();

	return 0;
}