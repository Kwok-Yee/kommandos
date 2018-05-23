#pragma once

class Bullet {
public:
	void Reset();
	irr::scene::ISceneNode* GetBullet();
	void SetBullet(irr::scene::ISceneNode* newBullet);
	Bullet();
private:
	irr::scene::ISceneNode* bullet;

};