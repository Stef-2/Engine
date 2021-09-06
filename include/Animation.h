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
        /*Animation(std::string name, double duration, double ticksPerSecond,
                  std::vector<Engine::VectorKeyFrame> positionKeyFrames,
                  std::vector<Engine::QuaternionKeyFrame> rotationKeyFrames,
                  std::vector<Engine::VectorKeyFrame> scaleKeyFrames);

        std::vector<Engine::VectorKeyFrame>& GetPositionKeyFrames();
        std::vector<Engine::QuaternionKeyFrame>& GetRotationKeyFrames();
        std::vector<Engine::VectorKeyFrame>& GetScaleKeyFrames();
        */
        std::string GetName();
        double GetDuration();
        double GetTicksPerSecond();
        double GetStartTime();

        //void SetPositionKeyFrames(std::vector<Engine::VectorKeyFrame> keyFrames);
        //void SetRotationKeyFrames(std::vector<Engine::QuaternionKeyFrame> keyFrames);
        //void SetScaleKeyFrames(std::vector<Engine::VectorKeyFrame> keyFrames);

        void SetName(std::string& name);
        void SetDuration(double duration);
        void SetTicksPerSecond(double ticksPerSecond);
        void SetStartTime(double startTime);

        void Setup();

    private:
        //std::vector<Engine::VectorKeyFrame> positionKeyFrames;
        //std::vector<Engine::QuaternionKeyFrame> rotationKeyFrames;
        //std::vector<Engine::VectorKeyFrame> scaleKeyFrames;

        std::string name;
        double duration;
        double ticksPerSecond;

        double startTime;
    };
}
#endif //  ANIMATION_H
