#ifndef ANIMATION_H
#define ANIMATION_H

#include "Node.h"

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

#include "vector"
#include "string"

namespace Engine
{
    // keyframe struct for vector (position, scale) types
    struct VectorKeyFrame
    {
        glm::vec3 value;
        double timestamp;
    };

    // keyframe struct for quaternion (rotation) types
    struct QuaternionKeyFrame
    {
        glm::quat value;
        double timestamp;
    };

    // a struct describing a set of transformations for a node at a given time
    struct NodeAnimation
    {
        // affected node
        Engine::Node* node;

        // transforms
        std::vector<VectorKeyFrame> positionKeyFrames;
        std::vector<QuaternionKeyFrame> rotationKeyFrames;
        std::vector<VectorKeyFrame> scaleKeyFrames;
    };

    class Animation
    {
    public:
        Animation();
        Animation(std::string name, double duration, double ticksPerSecond);
        Animation(std::string name, double duration, double ticksPerSecond, std::vector<Engine::NodeAnimation> nodeAnimations);
        
        std::vector<Engine::NodeAnimation>& GetNodeAnimations();
        std::string GetName();
        double GetDuration();
        double GetTicksPerSecond();
        double GetStartTime();

        void SetName(std::string& name);
        void SetDuration(double duration);
        void SetTicksPerSecond(double ticksPerSecond);
        void SetStartTime(double startTime);
        void SetNodeAnimations(std::vector<Engine::NodeAnimation> nodeAnimations);
        void AddNodeAnimation(Engine::NodeAnimation nodeAnimation);

        void Setup();

    private:
        std::vector<Engine::NodeAnimation> nodeAnimations;
        std::string name;
        double duration;
        double ticksPerSecond;

        double startTime;
    };
}
#endif //  ANIMATION_H
