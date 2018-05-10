#pragma once

class InputReceiver : public irr::IEventReceiver
{
public:

	// Check for input events 
	bool OnEvent(const irr::SEvent& event);

	// Check whether a key is being held down
	bool GetIsKeyDown(irr::EKEY_CODE keyCode);

	// Returns a vector3 of the mouse position
	irr::core::vector3df GetMousePosition();

	// Returns a boolean of the left mouse button pressed state
	bool GetIsLeftMouseButtonPressed();

	InputReceiver();

private:
	// Store the current state of each key
	bool isKeyDown[irr::KEY_KEY_CODES_COUNT];

	// Mouse position on the screen in 3D
	irr::core::vector3df mousePosition;

	// Mouse left button 
	bool isLeftMouseButtonPressed;
};