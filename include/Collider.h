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
		// aabb - aabb
		bool Intersects(Engine::BoundingBox& first, Engine::BoundingBox& second);
		// sphere - sphere
		bool Intersects(Engine::BoundingSphere& first, Engine::BoundingSphere& second);
		// capsule - capsule
		bool Intersects(Engine::BoundingCapsule& first, Engine::BoundingCapsule& second);

		// point - aabb
		bool Intersects(Engine::BoundingBox& box, Engine::Point& point);
		// point - sphere
		bool Intersects(Engine::BoundingSphere& sphere, Engine::Point& point);
		// point - capsule
		bool Intersects(Engine::BoundingCapsule& capsule, Engine::Point& point);

		// aabb - sphere
		bool Intersects(Engine::BoundingBox& box, Engine::BoundingSphere& sphere);
		// aabb - capsule
		bool Intersects(Engine::BoundingBox&, Engine::BoundingCapsule& capsule);
		// sphere - capsule

		template<typename T>
		bool Intersects(Engine::Triangle& first, Engine::Triangle& second);

		bool Contains(Engine::BoundingBox& larger, Engine::BoundingBox& smaller);
		bool Contains(Engine::BoundingSphere& larger, Engine::BoundingSphere& smaller);
		bool Contains(Engine::BoundingBox& larger, Engine::BoundingSphere& smaller);
		bool Contains(Engine::BoundingSphere& larger, Engine::BoundingBox& smaller);


	private:
		double gravity = 9.81;
	};
}
#endif