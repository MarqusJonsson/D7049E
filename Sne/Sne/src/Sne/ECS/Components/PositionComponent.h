#pragma once
#include "../../Math/Math.h"
struct PositionComponent
{
public:
	SneMath::vec3 pos;

	void Init(SneMath::vec3 pos)
	{
		this->pos = pos;
	}
};

