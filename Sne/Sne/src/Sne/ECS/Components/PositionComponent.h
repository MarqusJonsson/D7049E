#pragma once
struct PositionComponent
{
public:
	float x, y, z;

	void Init(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
};

