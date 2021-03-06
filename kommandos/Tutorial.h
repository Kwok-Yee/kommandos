#include <irrlicht.h>

#pragma once
///-------------------------------------------------------------------------------------------------
/// <summary>	Tutorial class which displays the buttons to use and the objective of the game. </summary>
///-------------------------------------------------------------------------------------------------
class Tutorial
{
public:
	bool isTutorialActive = true;
	///-------------------------------------------------------------------------------------------------
	/// <summary>	Shows the tutorial GUI. </summary>
	///-------------------------------------------------------------------------------------------------
	void ShowTutorial(irr::IrrlichtDevice* device);

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Removes the tutorial GUI. </summary>
	///-------------------------------------------------------------------------------------------------
	void RemoveTutorial();

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Update for tutorial GUI. </summary>
	///-------------------------------------------------------------------------------------------------
	void Update(irr::f32 frameDeltaTime);
};

