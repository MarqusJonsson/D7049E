#pragma once
#include "../Event.h"
#include "../../ECS/Entity.h"
struct MouseClickEvent : public Event
{
public:
	int button;

	MouseClickEvent(int button)
	{
		this->button = button;
	}
};