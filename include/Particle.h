#pragma once

#include "Mesh.h"

#include "glm/glm.hpp"

#include "vector"

namespace Engine
{
	// class encapsualtiong a simple, renderable particle with optional lifetime
	class Particle
	{
	public:
		// create a particle at a given position with optional lifetime specified in seconds
		// lifetime with a negative value will never expire
		Particle(glm::vec3 position, float lifeTime = -1.0f);

		~Particle();

		static std::vector<Engine::Particle*>& GetActiveParticles();

		// premade static utility meshes
		static Engine::SharedStaticMesh GetVertexMesh();
		static Engine::SharedStaticMesh GetTriangleMesh();
		static Engine::SharedStaticMesh GetQuadMesh();
		static Engine::SharedStaticMesh GetCubeMesh();
		static Engine::SharedStaticMesh GetSphereMesh();

		void MoveRelative(glm::vec3);
		void MoveAbsolute(glm::vec3);

		float GetDeathTime();
	private:
		Engine::SharedStaticMesh mesh;

		glm::vec3 position;

		// particle expiration time
		float mementoMori;

		// ==========================================================================
		// premade static utility meshes
		static Engine::SharedStaticMesh vertexMesh;
		static Engine::SharedStaticMesh triangleMesh;
		static Engine::SharedStaticMesh quadMesh;
		static Engine::SharedStaticMesh cubeMesh;
		static Engine::SharedStaticMesh sphereMesh;

		static std::vector<Engine::Particle*> particles;
	};
}