#pragma once

class Bullet {
public:
	void Reset();
	irr::scene::ISceneNode* GetBullet();
	void SetBullet(irr::scene::ISceneNode* newBullet);
	float GetDamage();
	void UpdateBullet(irr::core::vector3df mousePos, irr::core::vector3df playerPos, float frameDeltaTime);
	Bullet();
private:
	irr::scene::ISceneNode* bullet;
	bool setOnce = true;
	irr::core::vector3df velocity;
	irr::core::vector3df delta;
	float speed;
	float damage;
};