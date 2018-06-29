#pragma once

class Powerup
{
public:

	enum PowerupType
	{
		base, firerate, splitshot
	};

	void Reset();
	irr::scene::ISceneNode* GetNode();
	void SetPowerup(irr::scene::ISceneNode* newPowerup);
	irr::s32 GetPowerupType();
	Powerup();
	void SetPowerupType(irr::s32 powType);

private:

	irr::scene::ISceneNode* powerup;
	PowerupType type;

};

