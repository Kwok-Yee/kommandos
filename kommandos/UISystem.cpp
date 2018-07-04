#include "UISystem.h"
#include <irrlicht.h>
#include "EnemySpawner.h"
#include "Player.h"
#include "Game.h"

using namespace irr;
using namespace gui;
using namespace core;
using namespace scene;
using namespace video;

Game* _Game;
Player* _Player;

IGUIEnvironment* GUI_UISystem;
IGUIFont* font2_UI;
IGUIImage* rapidImg;
ITexture* rapidFireTexture;
IVideoDriver* UIdriver;

EGUI_ALIGNMENT upperleft_UI, upperight_UI;

void UISystem::InitUISystem(irr::IrrlichtDevice * device)
{
	_Game = _Game->GetInstance();
	_Player = _Game->GetPlayer();

	UIdriver = _Game->device->getVideoDriver();
	GUI_UISystem = _Game->device->getGUIEnvironment();

	inActiveRapidColor = SColor((u32)50, (u32)255, (u32)0, (u32)0);
	activeRapidColor = SColor((u32)255, (u32)255, (u32)255, (u32)255);

	rapidFireTexture = UIdriver->getTexture("../media/Textures/rapid.png");
	rapidImg = GUI_UISystem->addImage(rapidFireTexture, position2d<int>(700, 500));
	rapidImg->setScaleImage(true);
	rapidImg->setMaxSize(dimension2du(50, 50));
	rapidUIColor = inActiveRapidColor;
}

void UISystem::WaveUI(IrrlichtDevice* device)
{
	//Showing the current wave
	font2_UI = device->getGUIEnvironment()->getFont("../media/Fonts/fontlucida.png");
	core::stringw waveNumberString = L"Current wave: ";
	waveNumberString += currentWave;

	//For drawing the wave timer in seconds
	int waveCooldownSeconds = waveCooldown / 60;
	core::stringw waveCountDownString = L"Time till wave: ";
	waveCountDownString += waveCooldownSeconds;

	//Big wave incoming string
	core::stringw bigWaveString = L"BIG WAVE INCOMING!!";	

	if (_Player->GetRapidFireTimer() > 0) 
	{
		rapidUIColor = activeRapidColor;
	}
	else
	{
		rapidUIColor = inActiveRapidColor;
	}

	rapidImg->setColor(rapidUIColor);

	//Drawing the strings:
	font2_UI->draw(stringw(waveNumberString),
		core::rect<s32>(600, 40, 200, 100),
		video::SColor(255, 255, 255, 255));

	if (waveChangeUI == true)
	{
		font2_UI->draw(stringw(waveCountDownString),
			core::rect<s32>(300, 40, 200, 100),
			video::SColor(255, 255, 255, 255));

		if ((currentWave + 1) % 5 == 0)
		{
			font2_UI->draw(stringw(bigWaveString),
				core::rect<s32>(325, 120, 200, 100),
				video::SColor(255, 255, 255, 255));
		}
	}

}

void UISystem::SetRapidUIColor(SColor color)
{
	rapidUIColor = color;
}

