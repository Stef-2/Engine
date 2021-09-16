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

		inline Engine::Node& GetNode();
		// local transform from mesh into bone space
		inline glm::mat4 GetTransform();
		// transform that combines local with inherited node transforms
		inline glm::mat4 GetGlobalTransform();
		// full global transform at a given animation time
		inline glm::mat4 GetGlobalTransformAnimated();
		// number of vertices affected by this bone
		inline unsigned int GetNumAffectedVertices();
		// ID by which affected vertices refer to their bones
		inline unsigned int GetID() const;

		inline void SetNode(Engine::Node& node);
		inline void SetTransform(glm::mat4 transform);
		inline void SetAnimationTime(double time);
		inline void SetNumAffectedVerts(unsigned int numAffectedVerts);
		inline void SetID(unsigned int id);

	private:

		Engine::Node* node;
		glm::mat4 transform;
		double animationTime;
		unsigned int numAffectedVerts;
		unsigned int id;
	};
}

#endif // BONE_H