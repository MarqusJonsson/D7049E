#pragma once
#include "../Event.h"
#include "../../ECS/Entity.h"
// Not implemented yet, intended to occur on object collisions
struct CollisionEvent : public Event
{
	CollisionEvent(Entity entity1, Entity entity2)
	{
	}

	Entity entity;
};