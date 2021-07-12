#include "Monitor.h"

Engine::Monitor::Monitor()
{
    //ctor
}

Engine::Monitor::~Monitor()
{
    //dtor
}

Engine::Monitor::Monitor(const Monitor& other)
{
    //copy ctor
}

Engine::Monitor& Engine::Monitor::operator=(const Monitor& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    //assignment operator
    return *this;
}
