#pragma once
class Event
{
public:
    virtual~Event();

    virtual const char* type() const = 0;
};
