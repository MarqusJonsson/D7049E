#pragma once
#include <glm.hpp>
#include <gtx/transform.hpp>
#include <gtc/matrix_transform.hpp>
#include "btBulletDynamicsCommon.h"

namespace SneMath
{
	typedef glm::vec3 vec3;
	typedef glm::vec4 vec4;
	typedef glm::mat4 mat4;
	inline btVector3 vec3_to_btVector3(vec3 const& vec)
	{
		return { vec.x, vec.y, vec.z };
	}
	inline SneMath::vec3 btVector3_to_vec3(btVector3 const& vec)
	{
		return { vec.x(), vec.y(), vec.z() };
	}
	inline SneMath::mat4 translate(SneMath::mat4 const& mat, SneMath::vec3 const& vec)
	{
		return (SneMath::mat4) glm::translate(mat, vec);
	}
	inline SneMath::mat4 scale(SneMath::mat4 const& mat, SneMath::vec3 const& vec)
	{
		return (SneMath::mat4) glm::scale(mat, vec);
	}
	inline SneMath::mat4 rotate(SneMath::mat4 const& mat, float const& degree, SneMath::vec3 const& vec)
	{
		return (SneMath::mat4) glm::rotate(mat, glm::radians(degree), vec);
	}
	inline SneMath::mat4 btScalar_to_mat4(btScalar* matrix) {
		return SneMath::mat4(
			matrix[0], matrix[1], matrix[2], matrix[3],
			matrix[4], matrix[5], matrix[6], matrix[7],
			matrix[8], matrix[9], matrix[10], matrix[11],
			matrix[12], matrix[13], matrix[14], matrix[15]);
	}
	inline float floor(float const& value)
	{
		return glm::floor(value);
	}
}