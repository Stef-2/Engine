#ifndef COLLIDER_H
#define COLLIDER_H

#include "glm/glm.hpp"
#include "BoundingVolume.h"
#include "Mesh.h"

namespace Engine
{

	// Collider class, subsystem of Engine (Motor) class
	// responsible for holding physics related options and data,
	// collision detection, and other physics simulation operations
	class Collider
	{
	public:

		bool Intersects(Engine::BoundingBox& first, Engine::BoundingBox& second);
		bool Intersects(Engine::BoundingSphere& first, Engine::BoundingSphere& second);
		bool Intersects(Engine::BoundingBox& box, Engine::BoundingSphere& sphere);
		bool Intersects(Engine::Triangle& first, Engine::Triangle& second);

		bool Contains(Engine::BoundingBox& larger, Engine::BoundingBox& smaller);
		bool Contains(Engine::BoundingSphere& larger, Engine::BoundingSphere& smaller);
		bool Contains(Engine::BoundingBox& larger, Engine::BoundingSphere& smaller);
		bool Contains(Engine::BoundingSphere& larger, Engine::BoundingBox& smaller);


	private:
		float gravity = 9.81f;
	};
}
#endif