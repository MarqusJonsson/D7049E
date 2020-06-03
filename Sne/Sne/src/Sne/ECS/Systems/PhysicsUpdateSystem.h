#pragma once
#include "../BaseSystem.h"
#include "../../EventSystem/Events/PhysicsUpdateEvent.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../ManagerManager.h"
#include "../../EventSystem/EventBus.h"
#include "../../EventSystem/Events/PhysicsUpdateEvent.h"

// System for handling physics update events
class PhysicsUpdateSystem : public BaseSystem
{

public:
    PhysicsUpdateSystem()
    {
    }

    void EventSubscribe(EventBus* eventBus)
    {
        eventBus->subscribe(this, &PhysicsUpdateSystem::PhysicsUpdateEvent);
    }

    void PhysicsUpdateEvent(PhysicsUpdateEvent* physicsUpdateEvent)
    {
        //printf("physics update event triggered \n");
        physicsUpdateEvent->simulator->update(physicsUpdateEvent->timeStep, physicsUpdateEvent->maxSubSteps);

        for (auto& entity : entities)
        {
            TransformComponent& transformComponent = managerManager->GetComponent<TransformComponent>(entity);
            RigidBodyComponent& rigidBodyComponent = managerManager->GetComponent<RigidBodyComponent>(entity);
            
            btRigidBody* rigidBody = rigidBodyComponent.getRigidBody();
            const SneMath::vec3 scale = SneMath::btVector3_to_vec3(rigidBody->getCollisionShape()->getLocalScaling());
            btTransform btTransform;
            rigidBody->getMotionState()->getWorldTransform(btTransform);
            btScalar unscaledTransform[16];
            btTransform.getOpenGLMatrix(unscaledTransform);
            SneMath::mat4 transform = SneMath::btScalar_to_mat4(unscaledTransform);
            transform = SneMath::scale(transform, scale);
            transformComponent.setTransform(transform);
        }
    }

    void setManagerManager(eastl::shared_ptr<ManagerManager> managerManager)
    {
        this->managerManager = managerManager;
    }
private:
    eastl::shared_ptr<ManagerManager> managerManager;
};