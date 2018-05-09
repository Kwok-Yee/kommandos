#include <irrlicht.h>
#include "InputReceiver.h"
#include <iostream>

using namespace std;
using namespace irr;

// InputReceiver's static variables
bool InputReceiver::isLeftMouseButtonDown = false;
core::vector3df InputReceiver::position = core::vector3df(0,0,0);

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

	return false;
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