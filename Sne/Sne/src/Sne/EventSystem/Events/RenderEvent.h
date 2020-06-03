#pragma once
#include "../Event.h"
// Event to signal the render system to render
struct RenderEvent : public Event
{
	RenderEvent()
	{
	}
};