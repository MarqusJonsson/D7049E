#pragma once
#include "../Event.h"
#include "../../ECS/Entity.h"
// fix the file name to MouseClickEvent instead of mouseClickEvent
struct MouseClickEvent : public Event
{
public:
	int button;

	MouseClickEvent(int button)
	{
		this->button = button;
	}
};