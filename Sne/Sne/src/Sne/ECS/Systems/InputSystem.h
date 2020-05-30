#pragma once
#include "../BaseSystem.h"
#include "../../EventSystem/EventBus.h"
#include "../../EventSystem/Events/MouseClickEvent.h"
#include "../../EventSystem/Events/KeyClickEvent.h"
#include <iostream>
class InputSystem : public BaseSystem
{

public:
    InputSystem()
    {
       // this->componentManager = componentManager;
    }
    void EventSubscribe(EventBus* eventBus)
    {
        eventBus->subscribe(this, &InputSystem::mouseClickEvent);
        eventBus->subscribe(this, &InputSystem::keyClickEvent);
    }

    void mouseClickEvent(MouseClickEvent* mouseClickEvent)
    {

        //GLFWwindow* window = mouseClickEvent->getGLFWwindow();
        printf("mouseClickEvent in InputSystem \n");
        //glfwSetMouseButtonCallback(mouseClickEvent->getGLFWwindow(), glfw_mouseInputCallback);

    }

    void keyClickEvent(KeyClickEvent* keyClickEvent)
    {
        printf("key = %i in InputSystem \n", keyClickEvent->key);

    }


};