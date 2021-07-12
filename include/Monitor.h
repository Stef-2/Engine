#ifndef MONITOR_H
#define MONITOR_H

namespace Engine
{

    class Monitor
    {
        Monitor();


        ~Monitor();

        Monitor(const Monitor& other);

        Monitor& operator=(const Monitor& rhs);
    };

}

#endif