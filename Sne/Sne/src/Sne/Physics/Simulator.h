#pragma once
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
		// Keep track of the shapes, we release memory at exit.
		// Make sure to re-use collision shapes among rigid bodies whenever possible!
		btAlignedObjectArray<btCollisionShape*> collisionShapes;
	public:
		Simulator();
		void setup();
		void setGravity(SneMath::vec3 const& gravity);
		btCollisionShape* createCuboidCollision(SneMath::vec3 const& extends);
		btCollisionShape* createSphereCollision(float const& radius);
		btCollisionShape* createCapsuleCollision(float const& radius, float const& height);
		btCollisionShape* createCylinderCollision(SneMath::vec3 const& extends);
		btCollisionShape* createConeCollision(float const& radius, float const& height);
		btRigidBody* createRigidBody(btCollisionShape* const collisionShape, btTransform const& collisionTransform, float const& _mass = 0.0f, SneMath::vec3 const& inertia = SneMath::vec3());
		void addToSimulation(btRigidBody* rigidBody);
		void update(double const& deltatime, int const& maxSubSteps = 1);
		void shutdown();
	private:
	};
}