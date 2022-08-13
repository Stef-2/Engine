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
	struct AnimatedVertexExtension
	{
		glm::ivec4 boneID;
		glm::vec4 boneWeight;
	};

	// ===========================================================================

	// triangle data for collision detection
	struct Triangle
	{
		Triangle(Vertex& x, Vertex& y,Vertex& z) : a(x), b(y), c(z) {};
		Triangle& operator=(const Triangle& other);

		bool operator==(const Triangle& other);

		Vertex& a;
		Vertex& b;
		Vertex& c;
	};

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
		unsigned int GetVertexArrayBuffer();
		unsigned int GetVertexBufferObject();
		unsigned int GetElementBufferObject();

		std::vector<Vertex>& GetVertices();
		std::vector<unsigned int>& GetIndices();
		std::vector<Triangle>& GetTriangles();
		Engine::Material& GetMaterial();
		Engine::BoundingBox& GetBoundingBox();

		void SetVertices(std::vector<Vertex> vertices);
		void SetIndices(std::vector<unsigned int> indices);
		void SetMaterial(const Material& material);
		void SetBoundingBox(glm::vec3 mins, glm::vec3 maxs);

		bool GetInstanceable() const;
		void SetInstanceable(bool value);
	private:


	protected:
		// mesh vertices
		std::vector<Vertex> vertices;
		// triangles for collision detection
		std::vector<Triangle> triangles;

		std::shared_ptr<Engine::Material> material;

		// node containing transformation data
		Engine::Node* node;

		Engine::BoundingBox boundingBox;

		// order in which vertices connect to form triangles
		std::vector<unsigned int> indices;

		// array, vertex buffer and element buffer objects for rendering
		unsigned int vertexArrayObject;
		unsigned int vertexBufferObject;
		unsigned int elementBufferObject;

		bool instanceable;
	};

	// ===========================================================================

	// animated mesh, extending the static one
	class AnimatedMesh : public Mesh
	{
		using Engine::Mesh::Mesh;

	public:
		AnimatedMesh(std::vector<Vertex> vertices,std::vector<AnimatedVertexExtension> vertexExtension, std::vector<unsigned int> indices, Engine::Skeleton skeleton, std::vector<Engine::Animation> animations);

		std::vector<Engine::Animation>& GetAnimations();
		std::vector<AnimatedVertexExtension>& GetAnimatedVertexExtensions();
		Engine::Skeleton& GetSkeleton();

		unsigned int GetAnimatedVertexBufferObject();

		void SetVertices(std::vector<AnimatedVertexExtension> vertices);
		void AddAnimation(Engine::Animation animation);
		void SetSkeleton(Engine::Skeleton& skelly);
		void AddAnimatedVertexExtension();

	private:
		// mesh vertices
		std::vector<AnimatedVertexExtension> vertexExtensions;

		std::vector<Engine::Animation> animations;

		// Mr.Skeltal
		Engine::Skeleton skeleton;

		// additional vertex buffer object for the animated vertex extension
		unsigned int animatedVertexBufferObject;
	};
	
	// typedef for shared meshes
	using SharedStaticMesh = Engine::Shared<Engine::Mesh>;
	using SharedAnimatedMesh = Engine::Shared<Engine::AnimatedMesh>;
}

#endif //  MESH_H
