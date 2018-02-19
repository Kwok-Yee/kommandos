#pragma once
#include <irrlicht.h>
using namespace irr;
using namespace core;
class Collision
{
public:
	struct TAABB
	{
		core::vector3df m_pos;
		float m_size;
	};

	Collision();
	~Collision();
	bool AABBtoAABB(const TAABB& tBox1, const TAABB& tBox2);
};

