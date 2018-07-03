#pragma once

class Player;
class Enemy {
public:
	enum EnemyType {
		basic,
		fast,
		tanky,
		matroshka
	};
	void Reset();

	irr::scene::ISceneNode* GetEnemySceneNode();
	bool IsDead();
	void SetEnemyType(EnemyType type, int nestAmount = 0);
	void SetEnemy(irr::scene::ISceneNode* newEnemy);
	void SetPlayer(Player* _player);

	void Update(float frameDeltaTime);
	void Attack();
	void TakeDamage(irr::f32 damage);

	Enemy(irr::IrrlichtDevice* device);
private:
	irr::scene::ISceneNode* enemy;
	irr::core::vector3df velocity;
	irr::core::vector3df delta;

	EnemyType enemyType;
	bool dead;
	float health;
	float speed;
	float damage;
	int nestingLvl;
};

