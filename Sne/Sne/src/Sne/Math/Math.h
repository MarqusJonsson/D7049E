#pragma once
#include <glm.hpp>
#include "btBulletDynamicsCommon.h"

namespace SneMath
{
	typedef glm::vec3 vec3;
	typedef glm::vec4 vec4;
	inline btVector3 vec3_to_btVector3(const vec3& vec)
	{
		return { vec.x, vec.y, vec.z };
	}
	inline SneMath::vec3 btVector3_to_vec3(const btVector3& vec)
	{
		return { vec.x(), vec.y(), vec.z() };
	}
}