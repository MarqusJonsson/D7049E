#pragma once
#include "../Component.h"
//#include <EASTL/functional.h>
class PositionComponent : public Component
{
private:
	int xpos, ypos, zpos;
public:
	int x() { return xpos; }
	int y() { return ypos; }
	int z() { return zpos; }


	void init() override
	{
		xpos = 0;
		ypos = 0;
		zpos = 0;

	}

	void update() override
	{
		xpos++;
		ypos++;
		zpos++;
	}
	void setPos(int x, int y, int z)
	{
		xpos = x;
		ypos = y;
		zpos = z;
	}
};