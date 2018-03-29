#ifdef _MSC_VER
// We'll also define this to stop MSVC complaining about sprintf().
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "Irrlicht.lib")
#endif

#include <irrlicht.h>
#include "InputReceiver.h"
#include <iostream>

using namespace irr;
using namespace std;

// InputReceiver's static variables
bool InputReceiver::isLeftMouseButtonDown = false;
core::vector3df InputReceiver::position = core::vector3df(0,0,0);
SEvent::SJoystickEvent InputReceiver::joystickState;

bool InputReceiver::OnEvent(const SEvent& event)
{
	if (event.EventType == EET_MOUSE_INPUT_EVENT)
	{
		switch (event.MouseInput.Event)
		{
		case EMIE_LMOUSE_PRESSED_DOWN:
			InputReceiver::isLeftMouseButtonDown = true;
			break;
		case EMIE_LMOUSE_LEFT_UP:
			InputReceiver::isLeftMouseButtonDown = false;
			break;
		case EMIE_MOUSE_MOVED:
			InputReceiver::position.X = event.MouseInput.X;
			// Stores the event its y mouse position to z position for 2d mouse position
			InputReceiver::position.Z = event.MouseInput.Y;
			break;
		default:
			break;
		}
	}

	// Remember whether each key is down or up
	if (event.EventType == irr::EET_KEY_INPUT_EVENT)
		keyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;

	// Store the state of the first connected joystick
	if (event.EventType == irr::EET_JOYSTICK_INPUT_EVENT
		&& event.JoystickEvent.Joystick == 0)
	{
		InputReceiver::joystickState = event.JoystickEvent;
	}
	return false;
}

irr::core::array<irr::SJoystickInfo> InputReceiver::GetJoystickInfo() {
	return joystickInfo;
}

void InputReceiver::CheckJoystickPresent(IrrlichtDevice* device) {
	if (device->activateJoysticks(joystickInfo))
	{
		cout << "Joystick support is enabled and " << joystickInfo.size() << " joystick(s) are present." << endl;

		for (u32 joystick = 0; joystick < joystickInfo.size(); ++joystick)
		{
			cout << "Joystick " << joystick << ":" << endl;
			cout << "\tName: '" << joystickInfo[joystick].Name.c_str() << "'" << endl;
			cout << "\tAxes: " << joystickInfo[joystick].Axes << endl;
			cout << "\tButtons: " << joystickInfo[joystick].Buttons << endl;
			cout << "\tHat is: ";

			switch (joystickInfo[joystick].PovHat)
			{
			case SJoystickInfo::POV_HAT_PRESENT:
				cout << "present" << endl;
				break;

			case SJoystickInfo::POV_HAT_ABSENT:
				cout << "absent" << endl;
				break;

			case SJoystickInfo::POV_HAT_UNKNOWN:
			default:
				cout << "unknown" << endl;
				break;
			}
		}
	}
	else
	{
		cout << "Joystick support is not enabled." << endl;
	}
}

// This is used to check whether a key is being held down
bool InputReceiver::IsKeyDown(EKEY_CODE keyCode) const
{
	return keyIsDown[keyCode];
}

// We use this array to store the current state of each key
bool keyIsDown[KEY_KEY_CODES_COUNT];

InputReceiver::InputReceiver()
{
	for (u32 i = 0; i < KEY_KEY_CODES_COUNT; ++i)
		keyIsDown[i] = false;
}