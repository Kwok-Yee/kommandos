#include <irrlicht.h>
#include "Tutorial.h"

using namespace irr;
using namespace gui;
using namespace core;
using namespace scene;
using namespace video;

IGUIEnvironment* tutorialGUI;
IGUIImage* img;
int timer = 600;

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

void Tutorial::RemoveTutorial()
{
	img->remove();
	img = NULL;
}

void Tutorial::Update(irr::f32 frameDeltaTime)
{
	if (img) 
	{
		if (timer > 0) {
			timer -= frameDeltaTime;
		}
		else
		{
			RemoveTutorial();
		}
	}
}
