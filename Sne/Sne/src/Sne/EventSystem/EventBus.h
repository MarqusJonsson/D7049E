#pragma once
#include "EASTL/list.h"
#include "EASTL/map.h"
#include "HandlerFunctionBase.h"
#include <typeindex>
typedef eastl::list<HandlerFunctionBase*> HandlerList;
// EventBus keeps track of MemberFunctionHandlers and calls them when an event is published
class EventBus {
public:
    template<typename EventType>
    void publish(EventType* evnt) {
        HandlerList* handlers = subscribers[typeid(EventType)];

        if (handlers == nullptr) {
            return;
        }

        for (auto& handler : *handlers) {
            if (handler != nullptr) {
                handler->exec(evnt);
            }
        }
    }

    template<class T, class EventType>
    void subscribe(T* instance, void (T::* memberFunction)(EventType*)) {
        HandlerList* handlers = subscribers[typeid(EventType)];

        //First time initialization
        if (handlers == nullptr) {
            handlers = new HandlerList();
            subscribers[typeid(EventType)] = handlers;
        }

        handlers->push_back(new MemberFunctionHandler<T, EventType>(instance, memberFunction));
    }
private:
    eastl::map<std::type_index, HandlerList*> subscribers;
};