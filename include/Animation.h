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
        
        inline std::vector<Engine::NodeAnimation>& GetNodeAnimations();
        inline std::string GetName();
        inline double GetDuration();
        inline double GetTicksPerSecond();
        inline double GetStartTime();

        inline unsigned int GetCurrentPositionKey();
        inline unsigned int GetCurrentRotationKey();
        inline unsigned int GetCurrentScaleKey();

        inline void SetName(std::string& name);
        inline  void SetDuration(double duration);
        inline void SetTicksPerSecond(double ticksPerSecond);
        inline void SetStartTime(double startTime);
        inline void SetNodeAnimations(std::vector<Engine::NodeAnimation> nodeAnimations);
        inline void AddNodeAnimation(Engine::NodeAnimation nodeAnimation);

        inline void SetCurrentPositionKey(unsigned int key);
        inline void SetCurrentRotationKey(unsigned int key);
        inline void SetCurrentScaleKey(unsigned int key);

        inline void TransformNodes(double timeOffset);

    private:
        glm::mat4 TransformNode(Engine::NodeAnimation& nodeAnimation, double timeOffset);

        std::vector<Engine::NodeAnimation> nodeAnimations;
        std::string name;
        double duration;
        double ticksPerSecond;

        unsigned int currentPositionKey;
        unsigned int currentRotationKey;
        unsigned int currentScaleKey;

        double startTime;
    };
}
#endif //  ANIMATION_H
