#pragma once
#include "../../Math/Math.h"
struct TransformComponent
{
private:
	SneMath::mat4 M;
	SneMath::vec3 position;
	SneMath::vec3 scale;
	SneMath::vec3 rotation;
public:
	void Init(SneMath::vec3 position, SneMath::vec3 rotation, SneMath::vec3 scale)
	{
		this->position = position;
		this->rotation = rotation;
		this->scale = scale;
		M = SneMath::mat4(1.0f);
		M = SneMath::translate(M, position);
		M = SneMath::rotate(M, rotation.x, SneMath::vec3(1.0f, 0.0f, 0.0f));
		M = SneMath::rotate(M, rotation.y, SneMath::vec3(0.0f, 1.0f, 0.0f));
		M = SneMath::rotate(M, rotation.z, SneMath::vec3(0.0f, 0.0f, 1.0f));
		M = SneMath::scale(M, scale);
		return;
	}

	SneMath::mat4 getTransform()
	{
		return M;
	}

	void setTransform(SneMath::mat4 transform)
	{
		M = transform;
	}
};