#pragma once
#include "../Event.h"
#include "../../ECS/Entity.h"
struct DamageEvent: public Event
{
	DamageEvent(Entity affectedEntity)
	{
		entity = affectedEntity;
	}
	Entity entity;
	
};