#ifndef BONE_H
#define BONE_H

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

#include "string"
#include "vector"

namespace Engine
{
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



	class Bone
	{
	public:

	private:
		std::vector<PositionKey> positions;
		std::vector<RotationKey> rotations;
		std::vector<ScaleKey> scales;

		std::string name;
		glm::mat4 transform;
		unsigned int id;
	};
}

#endif //BONE_H