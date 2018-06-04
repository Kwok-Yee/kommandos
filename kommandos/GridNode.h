#pragma once
#include <irrlicht.h>
#include <math.h>


#define WORLD_SIZE 15

struct GridCell {
	irr::s32 xCoord, yCoord, id;
	GridCell *parent;
	irr::f32 g, h;

	GridCell() : parent(0){}
	GridCell(irr::s32 x, irr::s32 y, GridCell *_parent = 0) : xCoord(x), yCoord(y), parent(_parent), id(x * WORLD_SIZE + y), g(0), h(0) {};

	irr::f32 GetF() { return g + h; }
	irr::f32 ManHattanDistance(GridCell *endNode) {
		irr::f32 x = (float)(fabs((float)this->xCoord - endNode->xCoord));
		irr::f32 y = (float)(fabs((float)this->yCoord - endNode->yCoord));
		irr::f32 h = irr::core::max_(x , y);
		return h;
	}
};

