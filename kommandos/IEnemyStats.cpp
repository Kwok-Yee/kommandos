#include "IEnemyStats.h"
#include <iostream>

using namespace std;

#ifndef IENEMYSTATS_H
#define IENEMYSTATS_H
	class IEnemyStats 
	{
		void IEnemyStats::setSpeed(float s) {
			float speed;
			speed = s;
		}

		void IEnemyStats::setHealth(float h) {
			float health;
			health = h;
		}
	};

#endif
