#ifndef BONE_H
#define BONE_H

#include "Node.h"

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

#include "string"
#include "vector"

namespace Engine
{
	// animation keys
	struct PositionKey
	{
		glm::vec3 position;
		float timeStamp;
	};

	struct RotationKey
	{
		glm::quat orientation;
		float timeStamp;
	};

	struct ScaleKey
	{
		glm::vec3 scale;
		float timeStamp;
	};


	// Bone class used for animation
	// bone hierarchies are packed into an Engine::Skeleton, which is a component of any animated mesh
	class Bone
	{
	public:
		Bone();
		Bone(std::string name, glm::mat4 transform, unsigned int numAffectedVerts);

		std::string GetName();
		glm::mat4 GetTransform();
		std::vector<glm::mat4> GetInheritedTransforms();
		unsigned int GetNumAffectedVertices();
		unsigned int GetID();

		std::vector<PositionKey> GetPositions();
		std::vector<RotationKey> GetRotations();
		std::vector<ScaleKey> GetScales();

		void SetName(std::string name);
		void SetTransform(glm::mat4 transform);
		void AddInheritedTransform(glm::mat4 transform);
		void SetNumAffectedVerts(unsigned int numAffectedVerts);
		void SetID(unsigned int id);

		void SetPositions(std::vector<PositionKey> positions);
		void SetRotations(std::vector<RotationKey> rotations);
		void SetScales(std::vector<ScaleKey> scales);

	private:
		std::vector<PositionKey> positions;
		std::vector<RotationKey> rotations;
		std::vector<ScaleKey> scales;

		std::string name;
		glm::mat4 transform;
		std::vector<glm::mat4> inheritedTransforms;
		unsigned int numAffectedVerts;
		unsigned int id;
	};
}

#endif // BONE_H