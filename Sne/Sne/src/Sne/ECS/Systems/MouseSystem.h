#pragma once
#include "../BaseSystem.h"
#include "../../EventSystem/EventBus.h"
#include "../../EventSystem/Events/MouseClickEvent.h"
#include <iostream>
class MouseSystem : public BaseSystem
{

public:
    MouseSystem()
    {
       // this->componentManager = componentManager;
    }
    void EventSubscribe(EventBus* eventBus)
    {
        eventBus->subscribe(this, &MouseSystem::mouseClickEvent);
    }

    void mouseClickEvent(MouseClickEvent* mouseClickEvent)
    {

        //GLFWwindow* window = mouseClickEvent->getGLFWwindow();
        printf("mouseClickEvent in MouseSystem \n");
        //glfwSetMouseButtonCallback(mouseClickEvent->getGLFWwindow(), glfw_mouseInputCallback);

    }

};