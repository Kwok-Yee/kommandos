///-------------------------------------------------------------------------------------------------
// file:	Camera.cpp
//
// summary: Camera class for the topdown camera 
///-------------------------------------------------------------------------------------------------

#include <irrlicht.h>
#include "Camera.h"
#include <iostream>
#include <stdlib.h>
#include <ctime>
#include <algorithm>
#include "Player.h"
#include "GameOverState.h" //For future game over implementation
#include "Game.h"

using namespace irr;
using namespace core;
using namespace scene;

/// <summary>	The camera start position. </summary>
const vector3df cameraStartPosition = vector3df(0, 150, 0);
/// <summary>	The camera start target. </summary>
const vector3df cameraStartTarget = vector3df(0, 0, 0);

/// <summary>	The camera i device. </summary>
IrrlichtDevice* cameraIDevice;
/// <summary>	The camera scenemanager. </summary>
ISceneManager* cameraSmgr;
/// <summary>	The camera. </summary>
ICameraSceneNode* camera;
/// <summary>	The Player. </summary>
Player* player_;

Game* game_;

/// <summary>	The new camera position. </summary>
vector3df newCameraPosition;


int maxTime = 100;
int shakeTimer;
f32 resetTime = 0;
bool shaking = true;

///-------------------------------------------------------------------------------------------------
/// <summary>	Constructor. </summary>
///-------------------------------------------------------------------------------------------------

Camera::Camera(IrrlichtDevice* device)
{
	cameraIDevice = device;
	cameraSmgr = cameraIDevice->getSceneManager();
	game_ = game_->GetInstance();
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
	shakeTimer = maxTime;
	srand(time(0));
	state = shaking;
}

void Camera::ScreenShake(f32 frameDeltaTime)
{
	if (shakeTimer > 0)
	{
		shakeTimer -= frameDeltaTime;
		resetTime += frameDeltaTime;
		camera->setTarget(player_->getCamFollowObject()->getPosition());
		newCameraPosition.X = player_->getCamFollowObject()->getPosition().X;
		newCameraPosition.Z = player_->getCamFollowObject()->getPosition().Z;
		camera->setPosition(newCameraPosition);

		const f32 dur = 0.6;
		const f32 intensity = 1.5;
		
		vector3df playerPos = player_->getPlayerObject()->getPosition();
		vector3df randomVec = vector3df(playerPos.X + rand() % 2 * intensity, playerPos.Y, playerPos.Z + rand() % 2 * intensity);
		
		if (resetTime <= dur) 
		{
			player_->getCamFollowObject()->setPosition(randomVec);
		}
		if (resetTime > dur) 
		{
			player_->getCamFollowObject()->setPosition(randomVec);
		}
		if (resetTime > dur * 2)
		{
			resetTime = 0;
		}
	}
	else 
	{
		shakeTimer = maxTime;
		state = normal;
	}
}


///-------------------------------------------------------------------------------------------------
/// <summary>	Update camera position relative to the player while game is playing and move camera to start pos when game is over </summary>
///-------------------------------------------------------------------------------------------------

void Camera::CameraUpdate(f32 frameDeltaTime)
{

	switch (state)
	{
	case normal:
		if (camera)
		{
			newCameraPosition.X = player_->getPlayerObject()->getPosition().X;
			newCameraPosition.Z = player_->getPlayerObject()->getPosition().Z;
			camera->setPosition(newCameraPosition);
			camera->setTarget(player_->getPlayerObject()->getPosition());
		}
		break;
	case shaking:
		ScreenShake(frameDeltaTime);
		break;
	case gameover:
		camera->setPosition(cameraStartPosition);
		camera->setTarget(cameraStartTarget);
		break;
	}
}


