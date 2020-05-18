#pragma once
#include "../BaseSystem.h"
#include "../../EventSystem/Events/DamageEvent.h"
//#include "../../EventSystem/EventBus.h"
#include "../../ECS/ComponentManager.h"
#include "../Components/HealthComponent.h"
class CombatSystem : public BaseSystem 
{
public:
    CombatSystem(ComponentManager componentManager)
    {
        this->componentManager = componentManager;
    }
    void EventSubscribe(EventBus* eventBus) 
    {
        eventBus->subscribe(this, &CombatSystem::DamageEvent);
    }

    void DamageEvent(DamageEvent* damageEvent) 
    {
        printf("damage event triggered \n");
        HealthComponent& healthComponent = componentManager.GetComponent<HealthComponent>(damageEvent->entity);
        healthComponent.health -= 1;
    }
};