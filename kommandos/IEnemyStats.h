#include <iostream>

using namespace std;

#ifndef IENEMYSTATS_H
#define IENEMYSTATS_H

namespace irr
{
	namespace scene
	{
		class IEnemyStats
		{
		public:
			virtual void IEnemyStats::setSpeed(float s) {
				speed = s;
			}

			virtual void IEnemyStats::setHealth(float h) {
				health = h;
			}
		protected:
			float health;
			float speed;
		};
	}
}
#endif

