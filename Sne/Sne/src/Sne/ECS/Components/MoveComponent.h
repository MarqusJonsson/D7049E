#pragma once
#include <btBulletCollisionCommon.h>
struct MoveComponent
{
	float xVel, yVel, zVel;

	void Init(float speed)
	{
		// random angle
		//float angle = RandomFloat01() * 3.1415926f * 2;

		// random movement speed between given min & max
		

		// velocity x & y components
		/*xVel = cosf(anglespeed;
		yVel = sinf(angle) * speed;
		zVel = sinf(angle) * speed;*/

		xVel = speed;
		yVel = speed;
		zVel = speed;
	}
};