#ifndef ANIMATION_H
#define ANIMATION_H

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

#include "vector"
#include "string"

namespace Engine
{
    // keyframe struct for vector types
    struct VectorKeyFrame
    {
        glm::vec3 value;
        float timestamp;
    };

    // keyframe struct for quaternion types
    struct QuaternionKeyFrame
    {
        glm::quat value;
        float timestamp;
    };

    // an array of transform keys for each node / bone
    struct AnimationNode
    {
        std::vector<Engine::VectorKeyFrame> positionKeys;
        std::vector<Engine::QuaternionKeyFrame> rotationKeys;
        std::vector<Engine::VectorKeyFrame> scaleKeys;

        std::string name;
    };

    class Animation
    {
    public:
        Animation();
        Animation(std::string name, float duration, float ticksPerSecond, AnimationNode nodes);

        Engine::AnimationNode& GetNodes();
        std::string GetName();
        float GetDuration();
        float GetTicksPerSecond();

        void SetNodes(Engine::AnimationNode& nodes);
        void SetName(std::string& name);
        void SetDuration(float duration);
        void SetTicksPerSecond(float ticksPerSecond);

        void Setup();

    private:
        Engine::AnimationNode nodes;

        std::string name;
        float duration;
        float ticksPerSecond;
    };
}
#endif //  ANIMATION_H
