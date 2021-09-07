#ifndef BONE_H
#define BONE_H

#include "Node.h"

#include "glm/glm.hpp"

#include "string"
#include "vector"

namespace Engine
{
	
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

		void SetName(std::string name);
		void SetTransform(glm::mat4 transform);
		void SetNumAffectedVerts(unsigned int numAffectedVerts);
		void SetID(unsigned int id);

	private:

		std::string name;
		glm::mat4 transform;
		unsigned int numAffectedVerts;
		unsigned int id;
	};
}

#endif // BONE_H