#pragma once
#include "../Event.h"
struct KeyClickEvent : public Event
{
public:
	int key;
	KeyClickEvent(int key)
	{
		this->key = key;
	}
private:
};
