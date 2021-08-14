#ifndef BONE_H
#define BONE_H

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

#include "string"
#include "vector"

namespace Engine
{
	//animation keys
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


	//you have been spooked by mr. skeltal
	class Bone
	{
	public:
		Bone();
		Bone(std::string name, glm::mat4 transform, unsigned int numAffectedVerts);

		std::string GetName();
		glm::mat4 GetTransform();
		unsigned int GetNumAffectedVertices();
		unsigned int GetID();

		std::vector<PositionKey> GetPositions();
		std::vector<RotationKey> GetRotations();
		std::vector<ScaleKey> GetScales();

		void SetName(std::string name);
		void SetTransform(glm::mat4 transform);
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
		unsigned int numAffectedVerts;
		unsigned int id;
	};
}

#endif //BONE_H