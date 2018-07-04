#include "UISystem.h"
#include <irrlicht.h>
#include "EnemySpawner.h"

using namespace irr;
using namespace gui;
using namespace core;
using namespace scene;
using namespace video;

IGUIEnvironment* GUI_UISystem;
IGUIFont* font2_UI;

EGUI_ALIGNMENT upperleft_UI, upperight_UI;


void UISystem::WaveUI(IrrlichtDevice* device)
{
	//GUI_UISystem = device->getGUIEnvironment(); For possibly adding images later

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

