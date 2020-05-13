#pragma once
#include "../BaseSystem.h"
#include "../../EventSystem/Events/DamageEvent.h"
//#include "../../EventSystem/EventBus.h"
#include "../../ECS/ComponentManager.h"
#include "../Components/HealthComponent.h"
class CombatSystem : public BaseSystem 
{
public:
    void init() 
    {
        eventBus->subscribe(this, &CombatSystem::DamageEvent);
    }

    void DamageEvent(DamageEvent* damageEvent) 
    {
            
        HealthComponent& healthComponent = componentManager->GetComponent<HealthComponent>(damageEvent->entity);
        healthComponent.health -= 1;
    }
};