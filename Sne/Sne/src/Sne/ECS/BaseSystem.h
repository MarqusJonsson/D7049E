#pragma once
#include "Entity.h"
#include "EASTL/set.h"
#include "../EventSystem/EventBus.h"
#include "EASTL/unique_ptr.h"
#include "../ECS/EntityManager.h"
#include "../ECS/ComponentManager.h"
//#include "../ECS/SystemManager.h"
class BaseSystem
{
public:
	eastl::set<Entity> entities;
	EventBus* eventBus; //eastl::unique_ptr<EventBus> eventBus;
	eastl::unique_ptr<ComponentManager> componentManager;

};
