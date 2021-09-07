#ifndef BONE_H
#define BONE_H

#include "Node.h"

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

#include "string"
#include "vector"

namespace Engine
{
	// keyframe struct for vector types
	struct VectorKeyFrame
	{
		glm::vec3 value;
		double timestamp;
	};

	// keyframe struct for quaternion types
	struct QuaternionKeyFrame
	{
		glm::quat value;
		double timestamp;
	};


	// Bone class used for animation
	// bone hierarchies are packed into an Engine::Skeleton, which is a component of any animated mesh
	class Bone
	{
	public:
		Bone();
		Bone(std::string name, glm::mat4 transform, unsigned int numAffectedVerts);
		Bone(std::string name, glm::mat4 transform, unsigned int numAffectedVerts, unsigned int ID);

		std::string GetName();
		glm::mat4 GetTransform();
		unsigned int GetNumAffectedVertices();
		unsigned int GetID();

		std::vector<VectorKeyFrame>& GetPositionKeyFrames();
		std::vector<QuaternionKeyFrame>& GetRotationKeyFrames();
		std::vector<VectorKeyFrame>& GetScaleKeyFrames();

		void SetName(std::string name);
		void SetTransform(glm::mat4 transform);
		void SetNumAffectedVerts(unsigned int numAffectedVerts);
		void SetID(unsigned int id);

		void SetPositionKeyFrames(std::vector<VectorKeyFrame> positions);
		void SetRotationKeyFrames(std::vector<QuaternionKeyFrame> rotations);
		void SetScaleKeyFrames(std::vector<VectorKeyFrame> scales);

	private:
		std::vector<VectorKeyFrame> positionKeyFrames;
		std::vector<QuaternionKeyFrame> rotationKeyFrames;
		std::vector<VectorKeyFrame> scaleKeyFrames;

		std::string name;
		glm::mat4 transform;
		unsigned int numAffectedVerts;
		unsigned int id;
	};
}

#endif // BONE_H