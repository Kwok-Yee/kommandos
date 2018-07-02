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
}

void Camera::ScreenShake(f32 frameDeltaTime)
{
	if (shakeTimer > 0)
	{
		shakeTimer -= frameDeltaTime;
		resetTime += frameDeltaTime;
		camera->setTarget(player_->getPlayerObject()->getPosition());
		newCameraPosition.X = player_->getPlayerObject()->getPosition().X;
		newCameraPosition.Z = player_->getPlayerObject()->getPosition().Z;
		camera->setPosition(newCameraPosition);

		f32 dur = 0.6;
		
		vector3df randomVec = vector3df(rand() % 2, player_->getPlayerObject()->getPosition().Y, rand() % 2);

		std::cout << resetTime << std::endl;
		
		if (resetTime <= dur) 
		{
			player_->getPlayerObject()->setPosition(randomVec);
		}
		if (resetTime > dur) 
		{
			player_->getPlayerObject()->setPosition(randomVec);
		}
		if (resetTime > dur * 2)
		{
			resetTime = 0;
		}
	}
	else 
	{
		shakeTimer = maxTime;
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


