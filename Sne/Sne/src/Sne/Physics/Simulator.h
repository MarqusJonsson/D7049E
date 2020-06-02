#pragma once
#include <glm.hpp>
#include "btBulletDynamicsCommon.h"
#include "../Math/Math.h"
#include "EASTL/vector.h"

namespace Physics
{
	class Simulator
	{
	private:
		btDefaultCollisionConfiguration* collisionConfiguration;
		btCollisionDispatcher* dispatcher;
		btBroadphaseInterface* overlappingPairCache;
		btSequentialImpulseConstraintSolver* solver;
		btDiscreteDynamicsWorld* dynamicsWorld;
		//keep track of the shapes, we release memory at exit.
		//make sure to re-use collision shapes among rigid bodies whenever possible!
		btAlignedObjectArray<btCollisionShape*> collisionShapes;
	public:
		eastl::vector<SneMath::vec3> positions;
		eastl::vector<SneMath::vec3> rotations;
	public:
		Simulator();
		void setup();
		void setGravity(SneMath::vec3 gravity);
		void createCuboid(SneMath::vec3 const& extends, SneMath::vec3 const& position, float const& mass);
		void createShpere(float const& radius, SneMath::vec3 const& position, float const& mass);
		void createCapsule(float const& radius, float const& height, SneMath::vec3 const& position, float const& mass);
		void createCylinder(SneMath::vec3 const& extends, SneMath::vec3 const& position, float const& mass);
		void createCone(float const& radius, float const& height, SneMath::vec3 const& position, float const& mass);
		void update(double deltatime);
		void shutdown();
		void getPositions();
		void getRotations();
	private:
		void createCollider(btCollisionShape* const coliderShape, SneMath::vec3 const& position, float const& _mass = 0.0f);
	};
}