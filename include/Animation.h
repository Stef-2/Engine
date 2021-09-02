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
        Animation(std::vector<Engine::AnimationNode> nodes, std::string name, float duration, unsigned int ticksPerSecond);

        std::vector<Engine::AnimationNode>& GetNodes();
        std::string GetName();
        float GetDuration();
        unsigned int GetTicksPerSecond();

        void SetNodes(std::vector<Engine::AnimationNode>& nodes);
        void SetName(std::string& name);
        void SetDuration(float duration);
        void SetTicksPerSecond(unsigned int ticksPerSecond);

        void Setup();

    private:
        std::vector<Engine::AnimationNode> nodes;

        std::string name;
        float duration;
        unsigned int ticksPerSecond;
    };
}
#endif //  ANIMATION_H
