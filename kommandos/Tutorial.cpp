///-------------------------------------------------------------------------------------------------
// file:	Tutorial.cpp
//
// summary: Display the tutorial using GUI, makes use of 3 methods.
//
// author:	Nino Thijs
///-------------------------------------------------------------------------------------------------

#include <irrlicht.h>
#include "Tutorial.h"
#include "Game.h"

using namespace irr;
using namespace gui;
using namespace core;
using namespace scene;
using namespace video;

/// <summary> IGUIEnvironment object for starting the GUI. </summary>
IGUIEnvironment* tutorialGUI;

/// <summary> Image object for adding or removing an image. </summary>
IGUIImage* img;

/// <summary> Timer for keeping tutorial on screen (600/ 60 frames = 10 seconds) </summary>
int timer = 600;



///-------------------------------------------------------------------------------------------------
/// <summary>	Shows the tutorial GUI. </summary>
///-------------------------------------------------------------------------------------------------
void Tutorial::ShowTutorial(IrrlichtDevice* device)
{
	ISceneManager* smgr = device->getSceneManager();
	IVideoDriver* driver = device->getVideoDriver();

	stringw tempHeader = "Objective";
	stringw tempObjectiveText = "Survive and get points by killing enemies";
	const wchar_t* objectiveHeader = tempHeader.c_str();
	const wchar_t* objectiveText = tempObjectiveText.c_str();

	tutorialGUI = device->getGUIEnvironment();
	ITexture* tutorialImage = driver->getTexture("../media/Textures/Tutorial_Resize.png");
	tutorialGUI->addMessageBox(objectiveHeader, objectiveText);
	img = tutorialGUI->addImage(tutorialImage, position2d<int>(250, 90));
}

///-------------------------------------------------------------------------------------------------
/// <summary>	Removes the tutorial GUI. </summary>
///-------------------------------------------------------------------------------------------------
void Tutorial::RemoveTutorial()
{
	img->remove();
	img = NULL;
}


///-------------------------------------------------------------------------------------------------
/// <summary>	Update for tutorial GUI. </summary>
///-------------------------------------------------------------------------------------------------
void Tutorial::Update(irr::f32 frameDeltaTime)
{
	if (img)
	{
		if (timer > 0)
		{
			timer -= frameDeltaTime;
		}
		else
		{
			isTutorialActive = false;
			RemoveTutorial();
		}
	}
}

