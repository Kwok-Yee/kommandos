///-------------------------------------------------------------------------------------------------
// file:	Bullet.h
//
// summary:	Declares the bullet class
///-------------------------------------------------------------------------------------------------

#pragma once

///-------------------------------------------------------------------------------------------------
/// <summary>	A bullet. </summary>
///-------------------------------------------------------------------------------------------------

class Bullet {
public:
	///-------------------------------------------------------------------------------------------------
	/// <summary>	Bullet Modes. </summary>
	///-------------------------------------------------------------------------------------------------
	
	enum BulletMode {
		basic,
		rapidFire,
		splitFire
	};

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Set the Bullet Mode. </summary>
	///-------------------------------------------------------------------------------------------------

	void SetBulletMode(BulletMode mode);

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Resets this object. </summary>
	///-------------------------------------------------------------------------------------------------

	void Reset();

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Gets the bullet. </summary>
	///-------------------------------------------------------------------------------------------------

	irr::scene::ISceneNode* GetBullet();

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Sets a bullet. </summary>
	///-------------------------------------------------------------------------------------------------

	void SetBullet(irr::scene::ISceneNode* newBullet);

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Gets the damage. </summary>
	///-------------------------------------------------------------------------------------------------

	float GetDamage();

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Updates the bullet. </summary>
	///-------------------------------------------------------------------------------------------------

	void UpdateBullet(irr::core::vector3df mousePos, irr::core::vector3df playerPos, float frameDeltaTime);

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Sets the bullet speed. </summary>
	///-------------------------------------------------------------------------------------------------

	void SetSpeed(float s);

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Default constructor. </summary>
	///-------------------------------------------------------------------------------------------------
	
	irr::s32 GetBulletTimer();

	irr::s32 GetBulletLifeTimer();
	
	void SetBulletOffset(float o);

	Bullet();
private:
	/// <summary>	The bullet. </summary>
	irr::scene::ISceneNode* bullet;
	/// <summary>	The Bullet mode. </summary>
	BulletMode bulletMode;
	/// <summary>	True to set once. </summary>
	bool setOnce = true;
	/// <summary>	The velocity. </summary>
	irr::core::vector3df position;
	/// <summary>	The delta. </summary>
	irr::core::vector3df delta;
	/// <summary>	The speed. </summary>
	float speed;
	/// <summary>	The damage. </summary>
	float damage;
	/// <summary>	The bullet timer. </summary>
	irr::s32 bulletTimer;
	/// <summary>	The bullet life timer. </summary>
	irr::s32 bulletLifeTimer;
	/// <summary>	The bullet offset. </summary>
	float spread;
	irr::f32 length;
	irr::core::vector3df perpendicular;
	irr::core::vector3df offPos;
};