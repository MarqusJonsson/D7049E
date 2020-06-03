#include "Simulator.h"

// Constructor for a physics simulator, calls setup for a dynamic physics world
Physics::Simulator::Simulator()
{
	setup();
}

// Create dynamic physics world with gravity
void Physics::Simulator::setup()
{
	// collision configuration contains default setup for memory, collision setup.
	// Advanced users can create their own configuration.
	collisionConfiguration = new btDefaultCollisionConfiguration();
	// use the default collision dispatcher. For parallel processing you can use
	// a diffent dispatcher (see Extras / BulletMultiThreaded)
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	// btDbvtBroadphase is a good general purpose broadphase. You can also try
	// out btAxis3Sweep.
	overlappingPairCache = new btDbvtBroadphase();
	// the default constraint solver. For parallel processing you can use a
	// different solver (see Extras / BulletMultiThreaded)
	solver = new btSequentialImpulseConstraintSolver;
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
	dynamicsWorld->setGravity(btVector3(0,-10,0));
}

// Set the gravity of the simulators dynamic physics world
void Physics::Simulator::setGravity(SneMath::vec3 const& gravity)
{
	dynamicsWorld->setGravity(SneMath::vec3_to_btVector3(gravity));
}

// Create a cuboid collision shape and return a pointer to it
btCollisionShape* Physics::Simulator::createCuboidCollision(SneMath::vec3 const& extends)
{
	btCollisionShape* collisionShape = new btBoxShape(btVector3(btScalar(1.0f), btScalar(1.0f), btScalar(1.0f)));
	collisionShape->setLocalScaling(SneMath::vec3_to_btVector3(extends));
	collisionShapes.push_back(collisionShape);
	return collisionShape;
}

// Create a sphere collision shape and return a pointer to it
btCollisionShape* Physics::Simulator::createSphereCollision(float const& radius)
{
	btCollisionShape* collisionShape = new btSphereShape(btScalar(radius));
	collisionShapes.push_back(collisionShape);
	return collisionShape;
}

// Create a capsule collision shape and return a pointer to it
btCollisionShape* Physics::Simulator::createCapsuleCollision(float const& radius, float const& height)
{
	btCollisionShape* collisionShape = new btCapsuleShape(btScalar(radius), btScalar(height));
	collisionShapes.push_back(collisionShape);
	return collisionShape;
}

// Create a cylinder collision shape and return a pointer to it
btCollisionShape* Physics::Simulator::createCylinderCollision(SneMath::vec3 const& extends)
{
	btCollisionShape* collisionShape = new btCylinderShape(SneMath::vec3_to_btVector3(extends));
	collisionShapes.push_back(collisionShape);
	return collisionShape;
}

// Create a cone collision shape and return a pointer to it
btCollisionShape* Physics::Simulator::createConeCollision(float const& radius, float const& height)
{
	btCollisionShape* collisionShape = new btConeShape(btScalar(radius), btScalar(height));
	collisionShapes.push_back(collisionShape);
	return collisionShape;
}

/*
// Create a multi sphere collision shape and return a pointer to it (Not implemented yet)
btColllisionShape* Physics::Simulator::createMultiSphereShape(float const& radius, float const& height, SneMath::vec3 const& position, float const& mass)
{
	btCollisionShape* collisionShape = new btMultiSphereShape(...);
	return collisionShape;
}
*/

// Create a rigidbody from a collision shape and a collision transform and return a pointer to it
btRigidBody* Physics::Simulator::createRigidBody(btCollisionShape* const collisionShape, btTransform const& collisionTransform, float const& _mass, SneMath::vec3 const& inertia)
{
	btScalar mass(_mass);
	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = (mass != 0.0f);
	btVector3 localInertia = SneMath::vec3_to_btVector3(inertia);
	if (isDynamic)
		collisionShape->calculateLocalInertia(mass, localInertia);
	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' object
	btDefaultMotionState* motionState = new btDefaultMotionState(collisionTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, collisionShape, localInertia);
	btRigidBody* body = new btRigidBody(rbInfo);
	return body;
}

// Add a rigidbody to the simulator's dynamic physics world
void Physics::Simulator::addToSimulation(btRigidBody* rigidBody)
{
	//add the body to the dynamics world
	dynamicsWorld->addRigidBody(rigidBody);
}

// Update the state in the dynamic physics world, incrementing the time by a given time step
void Physics::Simulator::update(double const& timeStep, int const& maxSubSteps)
{
	// Update simulation
	dynamicsWorld->stepSimulation(btScalar(timeStep), maxSubSteps);
}

// Cleanup allocated memory used by the simulator
void Physics::Simulator::shutdown()
{
	// remove the rigidbodies from the dynamics world and delete them
	for (int i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
		{
		btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
		{
			delete body->getMotionState();
		}
		dynamicsWorld->removeCollisionObject(obj);
		delete obj;
	}
	// delete collision shapes
	for (int j = 0; j < collisionShapes.size(); j++)
	{
		btCollisionShape * shape = collisionShapes[j];
		collisionShapes[j] = 0;
		delete shape;
	}
	// delete dynamics world
	delete dynamicsWorld;
	// delete solver
	delete solver;
	// delete broadphase
	delete overlappingPairCache;
	// delete dispatcher
	delete dispatcher;
	delete collisionConfiguration;
	// next line is optional : it will be cleared by the destructor when the array goes out of scope
	collisionShapes.clear();
}
