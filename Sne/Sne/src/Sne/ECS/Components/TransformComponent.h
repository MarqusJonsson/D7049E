#include "glm.hpp"
#pragma once
struct TransformComponent
{
public:
	glm::mat4 M;
	glm::vec4 position;
	glm::vec4 scale;
	glm::vec4 rotation;

	void Init()
	{
		M = glm::mat4{ 1.0f };
	}
};