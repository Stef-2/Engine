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
		std::vector<glm::mat4>& GetInheritedTransforms();
		std::vector<std::string>& GetInheritedNames();
		unsigned int GetNumAffectedVertices();
		unsigned int GetID();

		std::vector<VectorKeyFrame>& GetPositions();
		std::vector<QuaternionKeyFrame>& GetRotations();
		std::vector<VectorKeyFrame>& GetScales();

		void SetName(std::string name);
		void SetTransform(glm::mat4 transform);
		void AddInheritedTransform(glm::mat4 transform);
		void AddInheritedName(std::string name);
		void SetNumAffectedVerts(unsigned int numAffectedVerts);
		void SetID(unsigned int id);

		void SetPositions(std::vector<VectorKeyFrame> positions);
		void SetRotations(std::vector<QuaternionKeyFrame> rotations);
		void SetScales(std::vector<VectorKeyFrame> scales);

	private:
		std::vector<VectorKeyFrame> positions;
		std::vector<QuaternionKeyFrame> rotations;
		std::vector<VectorKeyFrame> scales;

		std::string name;
		glm::mat4 transform;
		std::vector<glm::mat4> inheritedTransforms;
		std::vector<std::string> inheritedNames;
		unsigned int numAffectedVerts;
		unsigned int id;
	};
}

#endif // BONE_H