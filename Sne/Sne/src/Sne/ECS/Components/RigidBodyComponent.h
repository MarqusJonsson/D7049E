#pragma once
#include "../../Math/Math.h"
#include "btBulletDynamicsCommon.h"
struct RigidBodyComponent
{
private:
	btRigidBody* rigidBody;
public:
	void Init(btRigidBody* rigidBody)
	{
		this->rigidBody = rigidBody;
	}

	btRigidBody* getRigidBody()
	{
		return rigidBody;
	}
};