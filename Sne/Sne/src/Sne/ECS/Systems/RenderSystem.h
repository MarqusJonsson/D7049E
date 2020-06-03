#pragma once
#include "../BaseSystem.h"
#include "../../EventSystem/Events/RenderEvent.h"
#include "../Components/CuboidComponent.h"
#include "../Components/TransformComponent.h"
#include "../ManagerManager.h"
#include "../../EventSystem/EventBus.h"
#include "../../EventSystem/Events/RenderEvent.h"

// System for hadnling render events
class RenderSystem : public BaseSystem
{
public:
    RenderSystem()
    {
    }

    void EventSubscribe(EventBus* eventBus)
    {
        eventBus->subscribe(this, &RenderSystem::RenderEvent);
    }

    void RenderEvent(RenderEvent* renderEvent)
    {
        //printf("render event triggered \n");
        for (auto& entity : entities)
        {
            TransformComponent& transformComponent = managerManager->GetComponent<TransformComponent>(entity);
            CuboidComponent& cubeComponent = managerManager->GetComponent<CuboidComponent>(entity);

            // Set vertex and index buffer.
            bgfx::setVertexBuffer(0, cubeComponent.vbh);
            bgfx::setIndexBuffer(cubeComponent.ibh);

            // Set model matrix for rendering.
            bgfx::setTransform(&transformComponent.getTransform()[0]);

            // Submit primitive for rendering to view 0.
            bgfx::submit(0, cubeComponent.m_program);
        }
    }

    void setManagerManager(eastl::shared_ptr<ManagerManager> managerManager)
    {
        this->managerManager = managerManager;
    }
private:
    eastl::shared_ptr<ManagerManager> managerManager;
};