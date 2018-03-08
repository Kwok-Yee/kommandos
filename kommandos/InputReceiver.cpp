#ifdef _MSC_VER
// We'll also define this to stop MSVC complaining about sprintf().
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "Irrlicht.lib")
#endif

#include <irrlicht.h>
#include "InputReceiver.h"

using namespace irr;

struct InputReceiver::SMouseState {
	bool LeftButtonDown;
	SMouseState() : LeftButtonDown(false) { }
} MouseState;

// This is the one method that we have to implement
bool InputReceiver::OnEvent(const SEvent& event)
{
	// Remember the mouse state
	if (event.EventType == EET_MOUSE_INPUT_EVENT)
	{
		switch (event.MouseInput.Event)
		{
		case EMIE_LMOUSE_PRESSED_DOWN:
			MouseState.LeftButtonDown = true;
			break;
		case EMIE_LMOUSE_LEFT_UP:
			MouseState.LeftButtonDown = false;
			break;
		default:
			break;
		}
	}

	// Remember whether each key is down or up
	if (event.EventType == EET_KEY_INPUT_EVENT)
		KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;

	return false;
}

// This is used to check whether a key is being held down
bool InputReceiver::IsKeyDown(EKEY_CODE keyCode) const
{
	return KeyIsDown[keyCode];
}

const InputReceiver::SMouseState &InputReceiver::GetMouseState(void) const
{
	return MouseState;
}

// We use this array to store the current state of each key
bool KeyIsDown[KEY_KEY_CODES_COUNT];


InputReceiver::InputReceiver()
{
	for (u32 i = 0; i < KEY_KEY_CODES_COUNT; ++i)
		KeyIsDown[i] = false;
}