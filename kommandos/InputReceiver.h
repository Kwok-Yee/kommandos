#pragma once

class InputReceiver : public irr::IEventReceiver
{
public:
	// Mouse left button 
	static bool isLeftMouseButtonDown;

	// Mouse position on the screen
	static irr::core::vector3df position;

	// Joystick state
	static irr::SEvent::SJoystickEvent joystickState;

	// Returns an array with joystick info
	irr::core::array<irr::SJoystickInfo> GetJoystickInfo();
	
	// Checks for joystick presence and debug its info
	void CheckJoystickPresent(irr::IrrlichtDevice* device);

	// Check for input events 
	bool OnEvent(const irr::SEvent& event);

	// Check whether a key is being held down
	bool IsKeyDown(irr::EKEY_CODE keyCode) const;

	InputReceiver();

private:
	/* Store the current state of each key */
	bool keyIsDown[irr::KEY_KEY_CODES_COUNT];
	/* Store the joystick info in an array */
	irr::core::array<irr::SJoystickInfo> joystickInfo;
};