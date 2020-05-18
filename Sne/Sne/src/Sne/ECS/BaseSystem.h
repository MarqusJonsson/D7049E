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
	//eastl::unique_ptr<EntityManager> entityManager;
	ComponentManager componentManager;
	//eastl::unique_ptr<SystemManager> systemManager;
};
