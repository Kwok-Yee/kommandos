///-------------------------------------------------------------------------------------------------
// file:	Camera.h
//
// summary:	Header for Camera class
///-------------------------------------------------------------------------------------------------

#pragma once
#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace scene;

///-------------------------------------------------------------------------------------------------
/// <summary>	Top down camera that follows player </summary>
///-------------------------------------------------------------------------------------------------

class Camera
{
public:
	enum CameraState
	{
		normal,
		waveShaking,
		bigWaveShaking,
		shootShaking,
		gameover
	};

	CameraState state;
	static Camera* GetInstance(IrrlichtDevice* device);

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Constructor. </summary>
	///-------------------------------------------------------------------------------------------------

	Camera(irr::IrrlichtDevice* device);

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Updates the camera position and target </summary>
	///-------------------------------------------------------------------------------------------------

	void CameraUpdate(f32 frameDeltaTime);

private:

	 ///-------------------------------------------------------------------------------------------------
	 /// <summary>	Initializes the camera  </summary>
	 ///-------------------------------------------------------------------------------------------------
	 static Camera* instance;
	 void CameraInit();
	 void ScreenShake(f32 frameDeltaTim, f32 intensity);
};