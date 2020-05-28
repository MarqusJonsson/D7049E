#pragma once
#include "../BaseSystem.h"
#include "../../EventSystem/Events/DamageEvent.h"
#include "../Components/HealthComponent.h"
#include "../ECS/managerManager.h"

class CombatSystem : public BaseSystem 
{

public:
    CombatSystem()
    {
    }

   /* CombatSystem(ManagerManager managerManager)
    {
        this->managerManager = managerManager;
    }

    void Init(ManagerManager managerManager)
    {
        this->managerManager = managerManager;
    }*/

    void EventSubscribe(EventBus* eventBus) 
    {
        eventBus->subscribe(this, &CombatSystem::DamageEvent);
    }

    void DamageEvent(DamageEvent* damageEvent) 
    {
        printf("damage event triggered \n");
        HealthComponent& healthComponent = managerManager->GetComponent<HealthComponent>(damageEvent->entity);
        if (healthComponent.health - 1 <= 0) 
        {
            managerManager->DestroyEntity(damageEvent->entity);
        }
        else
        {
            healthComponent.health -= 1;
        }
    }

    void setManagerManager(eastl::shared_ptr<ManagerManager> managerManager)
    {
        this->managerManager = managerManager;
    }
private:
    eastl::shared_ptr<ManagerManager> managerManager;
};