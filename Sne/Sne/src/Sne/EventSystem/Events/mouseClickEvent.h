#pragma once
#include "../Event.h"
#include "../../ECS/Entity.h"
// fix the file name to MouseClickEvent instead of mouseClickEvent
struct MouseClickEvent : public Event
{
	MouseClickEvent(Entity entity)
	{
		this->entity = entity;
	}
private:
	Entity entity;
};