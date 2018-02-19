#include "Collision.h"
#include "irrlicht.h"

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif	

Collision::Collision()
{
}


Collision::~Collision()
{
}


bool Collision::AABBtoAABB(const TAABB& tBox1, const TAABB& tBox2)
{
	core::vector3df minPos1 = { tBox1.m_pos.X - tBox1.m_size.X / 2, tBox1.m_pos.Y - tBox1.m_size.Y / 2, tBox1.m_pos.Z - tBox1.m_size.Z / 2 };
	core::vector3df maxPos1 = { tBox1.m_pos.X + tBox1.m_size.X / 2, tBox1.m_pos.Y + tBox1.m_size.Y / 2, tBox1.m_pos.Z + tBox1.m_size.Z / 2 };
	core::vector3df minPos2 = { tBox2.m_pos.X - tBox2.m_size.X / 2, tBox2.m_pos.Y - tBox2.m_size.Y / 2, tBox2.m_pos.Z - tBox2.m_size.Z / 2 };
	core::vector3df maxPos2 = { tBox2.m_pos.X + tBox2.m_size.X / 2, tBox2.m_pos.Y + tBox2.m_size.Y / 2, tBox2.m_pos.Z + tBox2.m_size.Z / 2 };
	//Check if Box1's max is greater than Box2's min and Box1's min is less than Box2's max
	return(maxPos1.X > minPos2.X &&
			minPos1.X  < maxPos2.X &&
			maxPos1.Y > minPos2.Y &&
			minPos1.Y < maxPos2.Y &&
			maxPos1.Z > minPos2.Z &&
			minPos1.Z < maxPos2.Z );

	//If not, it will return false

}