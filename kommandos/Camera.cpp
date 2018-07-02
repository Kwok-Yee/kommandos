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

int shakeTimer = 600;
f32 lerpTime = 600;
f32 currentLerptime = 0;
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
}
vector3df Camera::Lerp(vector3df a, vector3df b, f32 t)
{
	return (1 - t)*a + t*b;
}
void Camera::ScreenShake(f32 frameDeltaTime)
{
	if (shakeTimer > 0)
	{
		shakeTimer -= frameDeltaTime;
		currentLerptime += frameDeltaTime;
		resetTime += frameDeltaTime;
		//camera->setTarget(player_->getPlayerObject()->getPosition());
		newCameraPosition.X = player_->getPlayerObject()->getPosition().X;
		newCameraPosition.Z = player_->getPlayerObject()->getPosition().Z;
		camera->setPosition(newCameraPosition);
		if (currentLerptime >= lerpTime) 
		{
			currentLerptime = lerpTime;
		}
		f32 percentage = currentLerptime / lerpTime;

		std::cout << resetTime << std::endl;

		if (resetTime <= 0.6) 
		{
			//camera->setPosition(lerp(player_->getPlayerObject()->getPosition() + vector3df(0, 0, 0), player_->getPlayerObject()->getPosition() + vector3df(0, 0, 10), percentage * 100));
			camera->setTarget(lerp(player_->getPlayerObject()->getPosition(), player_->getPlayerObject()->getPosition() + vector3df(1, 0, 0), percentage * 1000));
		}
		if (resetTime > 0.6) 
		{
			//camera->setPosition(lerp(player_->getPlayerObject()->getPosition() + vector3df(0, 80, 10), player_->getPlayerObject()->getPosition() + vector3df(0, 80, 0), percentage * 100));
			camera->setTarget(lerp(player_->getPlayerObject()->getPosition(), player_->getPlayerObject()->getPosition() + vector3df(1, 0, 0), percentage * 1000));
		}
		if (resetTime > 1.2)
		{
			resetTime = 0;
		}

	}
	else 
	{
		shakeTimer = 600;
		shaking = false;
	}
}


///-------------------------------------------------------------------------------------------------
/// <summary>	Update camera position relative to the player while game is playing and move camera to start pos when game is over </summary>
///-------------------------------------------------------------------------------------------------

void Camera::CameraUpdate(f32 frameDeltaTime)
{
	if (game_->GetIsGameOver() == true)
	{
		camera->setPosition(cameraStartPosition);
		camera->setTarget(cameraStartTarget);
	}
	else
	{
		if (shaking)
		{
			ScreenShake(frameDeltaTime);
		}
		else 
		{
			if (camera) 
			{
				newCameraPosition.X = player_->getPlayerObject()->getPosition().X;
				newCameraPosition.Z = player_->getPlayerObject()->getPosition().Z;
				camera->setPosition(newCameraPosition);
				camera->setTarget(player_->getPlayerObject()->getPosition());
			}	
		}
	}
}


