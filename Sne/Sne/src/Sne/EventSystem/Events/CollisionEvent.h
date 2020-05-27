#pragma once
#include "../Event.h"
#include "../../ECS/Entity.h"
struct CollisionEvent : public Event
{
	CollisionEvent(Entity entity1, Entity entity2)
	{

	}
	Entity entity;

};