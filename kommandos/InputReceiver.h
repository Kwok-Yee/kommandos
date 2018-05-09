#pragma once

class InputReceiver : public irr::IEventReceiver
{
public:
	// Mouse left button 
	static bool isLeftMouseButtonDown;

	// Mouse position on the screen
	static irr::core::vector3df position;

	// Check for input events 
	bool OnEvent(const irr::SEvent& event);

	// Check whether a key is being held down
	bool IsKeyDown(irr::EKEY_CODE keyCode) const;

	InputReceiver();

private:
	/* Store the current state of each key */
	bool keyIsDown[irr::KEY_KEY_CODES_COUNT];
};