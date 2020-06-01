#pragma once
#include "../../Math/Math.h"
struct TransformComponent
{
public:
	SneMath::mat4 M;
	SneMath::vec4 position;
	SneMath::vec4 scale;
	SneMath::vec4 rotation;

	void Init()
	{
		M = SneMath::mat4{1.0f};
	}
};