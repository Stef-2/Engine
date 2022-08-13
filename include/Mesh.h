#ifndef MESH_H
#define MESH_H

#define STB_IMAGE_IMPLEMENTATION

#include "Shader.h"
#include "Material.h"
#include "Skeleton.h"
#include "Animation.h"
#include "BoundingVolume.h"
#include "Node.h"
#include "Shared.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glfw3.h"

#include "iostream"
#include "vector"

namespace Engine
{
	// basic vertex data
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec3 bitangent;
		glm::vec3 tangent;
		glm::vec2 uv;
	};

	// ===========================================================================

	// vertex extension for animated meshes
	struct AnimatedVertex : public Vertex
	{
		glm::ivec4 boneID;
		glm::vec4 boneWeight;
	};

	// ===========================================================================

	// vertex extension for instanced meshes
	struct InstancedVertex : public Vertex
	{
		glm::mat4& InstancedTransform;

	};

	// vertex extension for instanced animated meshes
	struct InstancedAnimatedVertex : public AnimatedVertex, public InstancedVertex
	{
		// sizeof() reference

	};

	// ===========================================================================

	// triangle data for collision detection
	template<typename T>
	struct Triangle
	{
		Triangle(T& x, T& y, T& z) : a(x), b(y), c(z) {};
		Triangle& operator=(const Triangle<T>& other);

		bool operator==(const Triangle& other);

		T& a;
		T& b;
		T& c;
	};

	template<typename T>
	Triangle<T>& Engine::Triangle<T>::operator=(const Triangle<T>& other)
	{
		if (this != &other)
		{
			this->a = other.a;
			this->b = other.b;
			this->c = other.c;
		}
		return *this;
	}



	template<typename T>
	bool Engine::Triangle<T>::operator==(const Triangle& other)
	{
		return &this->a == &other.a &&
			&this->b == &other.b &&
			&this->c == &other.c;
	}

	// ===========================================================================

	// a static mesh class that stores vertex data to be used for rendering
	class Mesh
	{
	public:
		Mesh();

		Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);

		void Setup();

		// handles for Vertex Buffer and Element Buffer objects
		// they need to be passed to OpenGL functions that are supposed to draw the mesh
		unsigned int GetVAO();
		unsigned int GetVBO();
		unsigned int GetEBO();

		std::vector<Vertex>& GetVertices();
		std::vector<unsigned int>& GetIndices();
		std::vector<Triangle<Vertex>>& GetTriangles();
		Engine::Material& GetMaterial();
		Engine::BoundingBox& GetBoundingBox();

		void SetVertices(std::vector<Vertex> vertices);
		void SetIndices(std::vector<unsigned int> indices);
		void SetMaterial(const Material& material);
		void SetBoundingBox(glm::vec3 mins, glm::vec3 maxs);

		bool GetInstanceable() const;
		void SetInstanceable(bool value);

		template<typename T>
		void SetVertexFormat();
	private:
		// mesh vertices
		std::vector<Vertex> vertices;
		// triangles for collision detection
		std::vector<Triangle<Vertex>> triangles;

	protected:
		std::shared_ptr<Engine::Material> material;

		// node containing transformation data
		Engine::Node* node;

		Engine::BoundingBox boundingBox;

		// order in which vertices connect to form triangles
		std::vector<unsigned int> indices;

		// array, vertex buffer and element buffer objects for rendering
		unsigned int VAO;
		unsigned int VBO;
		unsigned int EBO;

		bool instanceable;
	};

	template<typename T>
	void Engine::Mesh::SetVertexFormat()
	{
		// bind the vertex buffer
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		// fill the vertex buffer with data
		glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(T), &vertices[0], GL_STATIC_DRAW);

		// setup vertex attributes

		// vertex positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(T), (void*)0);

		// vertex normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(T), (void*)offsetof(T, normal));

		// vertex bitangents
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(T), (void*)offsetof(T, bitangent));

		// vertex tangents
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(T), (void*)offsetof(T, tangent));

		// vertex texture coords (UVs)
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(T), (void*)offsetof(T, uv));

		// expand the vertex model depending on its type

		// animated vertices
		if constexpr(std::is_same<T, Engine::AnimatedVertex>() || std::is_same<T,Engine::InstancedAnimatedVertex>())
		{
			// vertex bone IDs
			glEnableVertexAttribArray(5);
			glVertexAttribIPointer(5, 4, GL_INT, sizeof(T), (void*)offsetof(T, boneID));

			// vertex bone weights
			glEnableVertexAttribArray(6);
			glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(T), (void*)offsetof(T, boneWeight));
		}

		// instanceable vertices
		if constexpr (std::is_same<T, Engine::InstancedVertex>() || std::is_same<T, Engine::InstancedAnimatedVertex>())
		{
			constexpr auto size = sizeof(Engine::InstancedVertex::InstancedTransform);
			constexpr auto quarter = size / 4;
			
			glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, size, (void*)(0 * quarter));
			glEnableVertexAttribArray(4);
			glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, size, (void*)(1 * quarter));
			glEnableVertexAttribArray(5);
			glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, size, (void*)(2 * quarter));
			glEnableVertexAttribArray(6);
			glVertexAttribPointer(10, 4, GL_FLOAT, GL_FALSE, size, (void*)(3 * quarter));

			glVertexAttribDivisor(7, 1);
			glVertexAttribDivisor(8, 1);
			glVertexAttribDivisor(9, 1);
			glVertexAttribDivisor(10, 1);
		}
	}

	// ===========================================================================

	// animated mesh, extending the static one
	class AnimatedMesh : public Mesh
	{
		using Engine::Mesh::Mesh;

	public:

		AnimatedMesh(std::vector<AnimatedVertex> vertices, std::vector<unsigned int> indices, Engine::Skeleton skeleton, std::vector<Engine::Animation> animations);

		std::vector<Engine::Animation>& GetAnimations();
		std::vector<AnimatedVertex>& GetVertices();
		std::vector<Triangle<AnimatedVertex>>& GetTriangles();
		Engine::Skeleton& GetSkeleton();

		void AddAnimation(Engine::Animation animation);
		void SetSkeleton(Engine::Skeleton& skelly);
		void Setup();

	private:
		// mesh vertices
		std::vector<AnimatedVertex> vertices;

		// triangles for collision detection
		std::vector<Triangle<AnimatedVertex>> triangles;

		std::vector<Engine::Animation> animations;

		// Mr.Skeltal
		Engine::Skeleton skeleton;
	};
	
	// typedef for shared meshes
	using SharedStaticMesh = Engine::Shared<Engine::Mesh>;
	using SharedAnimatedMesh = Engine::Shared<Engine::AnimatedMesh>;
}

#endif //  MESH_H
