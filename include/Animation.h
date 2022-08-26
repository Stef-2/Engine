#ifndef ANIMATION_H
#define ANIMATION_H

#include "Node.h"

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

#include "vector"
#include "string"

namespace Engine
{
	// 3D Animation

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

		unsigned int GetCurrentPositionKey();
		unsigned int GetCurrentRotationKey();
		unsigned int GetCurrentScaleKey();

		void SetName(std::string& name);
		void SetDuration(double duration);
		void SetTicksPerSecond(double ticksPerSecond);
		void SetStartTime(double startTime);
		void SetNodeAnimations(std::vector<Engine::NodeAnimation> nodeAnimations);
		void AddNodeAnimation(Engine::NodeAnimation nodeAnimation);

		void SetCurrentPositionKey(unsigned int key);
		void SetCurrentRotationKey(unsigned int key);
		void SetCurrentScaleKey(unsigned int key);

		void TransformNodes(double timeOffset);

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

	// 2D animation

	// utility struct that describes a UI element animation
	struct UIElementAnimation
	{
		// negative value operatior
		UIElementAnimation operator-() {
			UIElementAnimation negative { -this->translation, this->time }; return negative;
		}

		// addition operator
		UIElementAnimation& operator+(UIElementAnimation& other) {
			UIElementAnimation result {this->translation + other.translation, this->time + other.time};
			return result;
		}

		// subtraction operator
		UIElementAnimation& operator-(UIElementAnimation& other) {
			UIElementAnimation result{ this->translation - other.translation, this->time - other.time };
			return result;
		}

		glm::vec2 translation;

		// animation time in seconds
		float time;
	};
}

#endif //  ANIMATION_H
