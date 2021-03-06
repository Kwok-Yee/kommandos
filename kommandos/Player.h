///-------------------------------------------------------------------------------------------------
// file:	Player.h
//
// summary:	Declares the player class
///-------------------------------------------------------------------------------------------------

#pragma once
#include <irrlicht.h>
#include "InputReceiver.h"
#include "EnemySpawner.h"

using namespace irr;
using namespace core;
using namespace scene;

///-------------------------------------------------------------------------------------------------
/// <summary>	A player. </summary>
///-------------------------------------------------------------------------------------------------

class Player
{
public:

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Constructor. </summary>
	///-------------------------------------------------------------------------------------------------

	Player(irr::IrrlichtDevice* device);

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Take damage. </summary>
	///-------------------------------------------------------------------------------------------------

	void TakeDamage(irr::f32 damage);
	//Draws the healtbar in the top-left corner

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Draw health bar. </summary>
	///-------------------------------------------------------------------------------------------------

	void DrawHealthBar();
	//Moves player with WASD keys

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Moves the given input receiver. </summary>
	///-------------------------------------------------------------------------------------------------

	void Move(InputReceiver inputReceiver);
	//Shoots a bullet with right mouse clcik

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Shoots. </summary>
	///-------------------------------------------------------------------------------------------------

	void Shoot(InputReceiver inputReceiver, EnemySpawner* enemies);
	//Retruns the player object

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Gets player object. </summary>
	///-------------------------------------------------------------------------------------------------

	irr::scene::ISceneNode* getPlayerObject();
	irr::scene::ISceneNode* getCamFollowObject();

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Raycasts. </summary>
	///-------------------------------------------------------------------------------------------------

	void Raycast(irr::core::vector3df endPosition, irr::scene::ICameraSceneNode* camera);

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Executes the line intersect action. </summary>
	///-------------------------------------------------------------------------------------------------

	bool OnLineIntersect(irr::core::plane3df &plane, irr::core::line3df &ray);

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Gets vulnerable timer. </summary>
	///-------------------------------------------------------------------------------------------------

	irr::s32 getVulnerableTimer();

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Gets mouse position. </summary>
	///-------------------------------------------------------------------------------------------------

	irr::core::vector3df GetMousePosition();

	s32 GetRapidFireTimer();

	s32 GetSplitFireTimer();

private:
	///-------------------------------------------------------------------------------------------------
	/// <summary>	Initializes this object. </summary>
	///-------------------------------------------------------------------------------------------------

	void Init();
	/// <summary>	The bullet timer. </summary>
	irr::s32 bulletTimer;
	/// <summary>	The rapid fire power up timer. </summary>
	irr::s32 rapidFireTimer;
	/// <summary>	The split fire power up timer. </summary>
	irr::s32 splitFireTimer;
};

