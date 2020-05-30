#pragma once
#include "../Event.h"
#include "../../ECS/Entity.h"
#include <GLFW\glfw3.h>
// fix the file name to MouseClickEvent instead of mouseClickEvent
struct MouseClickEvent : public Event
{
	MouseClickEvent(Entity entity, GLFWwindow* window)
	{
		this->entity = entity;
		printf("MouseClickEvent created \n");
	}

	MouseClickEvent() 
	{

	}

	GLFWwindow* getGLFWwindow() 
	{
		return window;
	}
private:
	Entity entity;
	GLFWwindow* window;
};