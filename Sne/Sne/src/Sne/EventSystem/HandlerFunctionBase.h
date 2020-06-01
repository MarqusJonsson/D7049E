#pragma once
#include "Event.h"
// This is the interface for MemberFunctionHandler that each specialization will use
class HandlerFunctionBase {
public:
    // Call the member function
    void exec(Event* evnt) {
        call(evnt);
    }
private:
    // Implemented by MemberFunctionHandler
    virtual void call(Event* evnt) = 0;
};

