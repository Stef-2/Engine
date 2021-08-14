#ifndef ANIMATION_H
#define ANIMATION_H


#include "string"

namespace Engine
{

    class Animation
    {
    public:
        Animation();

    private:
        std::string name;
        float duration;
        unsigned int ticksPerSecond;
    };
}
#endif // ANIMATION_H
