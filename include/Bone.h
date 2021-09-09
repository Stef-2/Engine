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
		Bone(glm::mat4 transform, unsigned int numAffectedVerts, unsigned int ID);
		Bone(Engine::Node& node, glm::mat4 transform, unsigned int numAffectedVerts, unsigned int ID);

		Engine::Node& GetNode();
		glm::mat4 GetTransform();
		unsigned int GetNumAffectedVertices();
		unsigned int GetID() const;

		void SetNode(Engine::Node& node);
		void SetTransform(glm::mat4 transform);
		void SetNumAffectedVerts(unsigned int numAffectedVerts);
		void SetID(unsigned int id);

	private:

		Engine::Node* node;
		glm::mat4 transform;
		unsigned int numAffectedVerts;
		unsigned int id;
	};
}

#endif // BONE_H