#ifndef ANIMATION_H
#define ANIMATION_H

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

#include "vector"
#include "string"

namespace Engine
{

    class Animation
    {
    public:
        Animation();
        Animation(std::string name, double duration, double ticksPerSecond);
        
        std::string GetName();
        double GetDuration();
        double GetTicksPerSecond();
        double GetStartTime();

        void SetName(std::string& name);
        void SetDuration(double duration);
        void SetTicksPerSecond(double ticksPerSecond);
        void SetStartTime(double startTime);

        void Setup();

    private:
        std::string name;
        double duration;
        double ticksPerSecond;

        double startTime;
    };
}
#endif //  ANIMATION_H
