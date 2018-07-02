///-------------------------------------------------------------------------------------------------
// file:	InputReceiver.cpp
//
// summary: InputReceiver is used for listening to user input, mouse clicks and positions.
///-------------------------------------------------------------------------------------------------

#include <irrlicht.h>
#include "InputReceiver.h"
#include <iostream>

using namespace std;
using namespace irr;
using namespace core;

bool InputReceiver::OnEvent(const SEvent& event)
{
	if (event.EventType == EET_MOUSE_INPUT_EVENT)
	{
		switch (event.MouseInput.Event)
		{
		case EMIE_LMOUSE_PRESSED_DOWN:
			isLeftMouseButtonPressed = true;
			break;
		case EMIE_LMOUSE_LEFT_UP:
			isLeftMouseButtonPressed = false;
			break;
		case EMIE_MOUSE_MOVED:
			mousePosition.X = event.MouseInput.X;
			// Stores the event its y mouse position to z position
			mousePosition.Z = event.MouseInput.Y;
			break;
		default:
			break;
		}
	}

	// Remember whether each key is down or up
	if (event.EventType == irr::EET_KEY_INPUT_EVENT)
	{
		isKeyDown[event.KeyInput.Key] = event.KeyInput.PressedDown;
	}
	return false;
}

// This is used to check whether a key is being held down
bool InputReceiver::GetIsKeyDown(EKEY_CODE keyCode)
{
	return isKeyDown[keyCode];
}

vector3df InputReceiver::GetMousePosition() 
{
	return mousePosition;
}

bool InputReceiver::GetIsLeftMouseButtonPressed() 
{
	return isLeftMouseButtonPressed;
}

InputReceiver::InputReceiver()
{
	// Set all states of the keys to be "not pressed"
	for (u32 i = 0; i < KEY_KEY_CODES_COUNT; ++i)
	{
		isKeyDown[i] = false;
	}
}