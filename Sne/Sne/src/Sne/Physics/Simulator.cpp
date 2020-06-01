#include "Simulator.h"

#include <stdio.h>

// Create dynamic physics world with gravity
Physics::Simulator::Simulator()
{
	setup();
}

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

void Physics::Simulator::setGravity(SneMath::vec3 gravity)
{
	dynamicsWorld->setGravity(SneMath::vec3_to_btVector3(gravity));
}

void Physics::Simulator::createCollider(btCollisionShape* const coliderShape, SneMath::vec3 const& position, float const& _mass)
{
	collisionShapes.push_back(coliderShape);
	btTransform coliderTransform;
	coliderTransform.setIdentity();
	coliderTransform.setOrigin(SneMath::vec3_to_btVector3(position));
	btScalar mass(_mass);
	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = (mass != 0.0f);
	btVector3 localInertia(0, 0, 0);
	if (isDynamic)
		coliderShape->calculateLocalInertia(mass, localInertia);
	//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState(coliderTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, coliderShape, localInertia);
	btRigidBody* body = new btRigidBody(rbInfo);
	//add the body to the dynamics world
	dynamicsWorld->addRigidBody(body);
}

void Physics::Simulator::createCuboid(SneMath::vec3 const& extends, SneMath::vec3 const& position, float const& mass)
{
	btCollisionShape* coliderShape = new btBoxShape(SneMath::vec3_to_btVector3(extends));
	createCollider(coliderShape, position, mass);
}

void Physics::Simulator::createShpere(float const& radius, SneMath::vec3 const& position, float const& mass)
{
	btCollisionShape* coliderShape = new btSphereShape(btScalar(radius));
	createCollider(coliderShape, position, mass);
}

void Physics::Simulator::createCapsule(float const& radius, float const& height, SneMath::vec3 const& position, float const& mass)
{
	btCollisionShape* coliderShape = new btCapsuleShape(btScalar(radius), btScalar(height));
	createCollider(coliderShape, position, mass);
}

void Physics::Simulator::createCylinder(SneMath::vec3 const& extends, SneMath::vec3 const& position, float const& mass)
{
	btCollisionShape* coliderShape = new btCylinderShape(SneMath::vec3_to_btVector3(extends));
	createCollider(coliderShape, position, mass);
}

void Physics::Simulator::createCone(float const& radius, float const& height, SneMath::vec3 const& position, float const& mass)
{
	btCollisionShape* coliderShape = new btConeShape(btScalar(radius), btScalar(height));
	createCollider(coliderShape, position, mass);
}

/*
void Physics::Simulator::createMultiSphereShape(float const& radius, float const& height, SneMath::vec3 const& position, float const& mass)
{
	btCollisionShape* coliderShape = new btMultiSphereShape((btScalar(radius), btScalar(height));
	createCollider(coliderShape, position);
}
*/

void Physics::Simulator::update(double timeStep)
{
	// Update simulation
	dynamicsWorld->stepSimulation(timeStep, 1);
	// print positions of all objects
	for (int j = dynamicsWorld->getNumCollisionObjects() - 1; j >= 0; j--)
	{
		btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[j];
		btRigidBody* body = btRigidBody::upcast(obj);
		btTransform trans;
		if (body && body->getMotionState())
		{
			body->getMotionState()->getWorldTransform(trans);
		}
		else
		{
			trans = obj->getWorldTransform();
		}
		printf(" world pos object %d = %f ,%f ,%f\n", j, float(trans.getOrigin().getX()), float(
			trans.getOrigin().getY()), float(trans.getOrigin().getZ()));
	}
}

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
