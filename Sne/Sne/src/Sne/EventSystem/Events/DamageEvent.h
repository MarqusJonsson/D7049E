#pragma once
#include "../Event.h"
#include "../../ECS/Entity.h"
// Minimalistic damage event containing the affected entity
struct DamageEvent: public Event
{
	DamageEvent(Entity affectedEntity)
	{
		entity = affectedEntity;
	}
	Entity entity;
	
};