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

	 void CameraInit();
	 vector3df Lerp(vector3df a, vector3df b, f32 t);
	 void ScreenShake(f32 frameDeltaTime);
};