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
#include "SoundManager.h"

using namespace irr;
using namespace core;
using namespace scene;

#define EARTHQUAKE_SOUND "../media/Sounds/equake4.mp3"

/// <summary>	The camera start position. </summary>
const vector3df cameraStartPosition = vector3df(0, 150, 0);
/// <summary>	The camera start target. </summary>
const vector3df cameraStartTarget = vector3df(0, 0, 0);

SoundManager* _soundManager;
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

const int maxTimeNormalShake = 100;
const int shotTimeShake = 10;
int shakeTimer;
f32 resetTime = 0;
bool soundPlaying = false;
bool isShaking = false;

///-------------------------------------------------------------------------------------------------
/// <summary>	Constructor. </summary>
///-------------------------------------------------------------------------------------------------

Camera* Camera::instance = 0;

Camera * Camera::GetInstance(IrrlichtDevice* device)
{
	if (!instance)
	{
		instance = new Camera(device);
	}
	return instance;
}

Camera::Camera(IrrlichtDevice* device)
{
	cameraIDevice = device;
	cameraSmgr = cameraIDevice->getSceneManager();
	game_ = game_->GetInstance();
	CameraInit();
	_soundManager = _soundManager->GetInstance();
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
	//shakeTimer = maxTime;
	srand(time(0));
	state = normal;
}

void Camera::ScreenShake(f32 frameDeltaTime, f32 intensity)
{
	if (shakeTimer > 0)
	{
		shakeTimer -= frameDeltaTime;
		resetTime += frameDeltaTime;
		

		const f32 dur = 0.6;
		
		vector3df playerPos = player_->getPlayerObject()->getPosition();
		vector3df randomVec = vector3df(playerPos.X + rand() % 2 * intensity, playerPos.Y, playerPos.Z + rand() % 2 * intensity);
		
		if (resetTime <= dur) 
		{
			player_->getCamFollowObject()->setPosition(randomVec);
			camera->setTarget(player_->getCamFollowObject()->getPosition());
			newCameraPosition.X = player_->getCamFollowObject()->getPosition().X;
			newCameraPosition.Z = player_->getCamFollowObject()->getPosition().Z;
			camera->setPosition(newCameraPosition);
		}
		if (resetTime > dur) 
		{
			player_->getCamFollowObject()->setPosition(randomVec);
			camera->setTarget(player_->getCamFollowObject()->getPosition());
			newCameraPosition.X = player_->getCamFollowObject()->getPosition().X;
			newCameraPosition.Z = player_->getCamFollowObject()->getPosition().Z;
			camera->setPosition(newCameraPosition);
		}
		if (resetTime > dur * 2)
		{
			resetTime = 0;
		}
	}
	else 
	{
		//shakeTimer = maxTime;
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
		soundPlaying = false;
		isShaking = false;
		break;
	case waveShaking:
		if (!isShaking) 
		{
			isShaking = true;
			shakeTimer = maxTimeNormalShake;
		}
		ScreenShake(frameDeltaTime, 0.4);
		break;
	case bigWaveShaking:
		if (!isShaking)
		{
			isShaking = true;
			shakeTimer = maxTimeNormalShake;
		}
		ScreenShake(frameDeltaTime, 0.3);
		if (!soundPlaying)
		{
			soundPlaying = true;
			_soundManager->PlaySound(EARTHQUAKE_SOUND, false);
		}
		break;
	case shootShaking:
		if (!isShaking)
		{
			isShaking = true;
			shakeTimer = shotTimeShake;
		}
		ScreenShake(frameDeltaTime, 2);
		break;
	case gameover:
		camera->setPosition(cameraStartPosition);
		camera->setTarget(cameraStartTarget);
		break;
	}
}


