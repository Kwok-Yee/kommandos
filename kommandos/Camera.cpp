///-------------------------------------------------------------------------------------------------
// file:	Camera.cpp
//
// summary: Camera class for the topdown camera 
///-------------------------------------------------------------------------------------------------

#include <irrlicht.h>
#include "Camera.h"
#include <iostream>
#include "Player.h"
#include "GameOverState.h" //For future game over implementation

using namespace irr;
using namespace core;
using namespace scene;

/// <summary>	The camera start position. </summary>
const vector3df cameraStartPosition = vector3df(0, 120, 0);
/// <summary>	The camera start target. </summary>
const vector3df cameraStartTarget = vector3df(0, 0, 0);

/// <summary>	The camera i device. </summary>
IrrlichtDevice* CameraIDevice;
/// <summary>	The camera scenemanager. </summary>
ISceneManager* cameraSmgr;
/// <summary>	The camera. </summary>
ICameraSceneNode* camera;
/// <summary>	The Player. </summary>
Player* player_;
/// <summary>	The new camera position. </summary>
vector3df newCameraPosition;

///-------------------------------------------------------------------------------------------------
/// <summary>	Constructor. </summary>
///-------------------------------------------------------------------------------------------------

Camera::Camera(IrrlichtDevice* device)
{
	CameraIDevice = device;
	cameraSmgr = CameraIDevice->getSceneManager();
	CameraInit();
}

///-------------------------------------------------------------------------------------------------
/// <summary>	Initialization of the Camera </summary>
///-------------------------------------------------------------------------------------------------

void Camera::CameraInit()
{
	camera = cameraSmgr->addCameraSceneNode();
	if (camera)
	{
		camera->setPosition(cameraStartPosition);
		camera->setTarget(cameraStartTarget);
		newCameraPosition.Y = 80;
	}
}

///-------------------------------------------------------------------------------------------------
/// <summary>	Update camera position relative to the player </summary>
///-------------------------------------------------------------------------------------------------

void Camera::CameraUpdate()
{
  //Player *player;
	if (camera)
	{
		newCameraPosition.X = player_->getPlayerObject()->getPosition().X;
		newCameraPosition.Z = player_->getPlayerObject()->getPosition().Z;
		camera->setPosition(newCameraPosition);
		camera->setTarget(player_->getPlayerObject()->getPosition());
	}
}


