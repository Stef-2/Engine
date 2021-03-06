#ifndef MESH_H
#define MESH_H

#define STB_IMAGE_IMPLEMENTATION

#include "Shader.h"
#include "Material.h"
#include "Skeleton.h"
#include "Animation.h"
#include "BoundingVolume.h"
#include "Node.h"

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
 
    // ========================================================================

    // vertex extension for animated meshes
    struct VertexBoneData
    {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec3 bitangent;
        glm::vec3 tangent;
        glm::vec2 uv;
        glm::ivec4 boneID;
        glm::vec4 boneWeight;
    };

    // ========================================================================

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

    // ========================================================================

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
    };

    // ========================================================================

    // animated mesh, extending the static one
    class AnimatedMesh : public Mesh
    {
        using Engine::Mesh::Mesh;

    public:

        AnimatedMesh(std::vector<VertexBoneData> vertices, std::vector<unsigned int> indices, Engine::Skeleton skeleton, std::vector<Engine::Animation> animations);

        std::vector<Engine::Animation>& GetAnimations();
        std::vector<VertexBoneData>& GetVertices();
        std::vector<Triangle<VertexBoneData>>& GetTriangles();
        Engine::Skeleton& GetSkeleton();

        void AddAnimation(Engine::Animation animation);
        void SetSkeleton(Engine::Skeleton& skelly);
        void Setup();

    private:
        // mesh vertices
        std::vector<VertexBoneData> vertices;

        // triangles for collision detection
        std::vector<Triangle<VertexBoneData>> triangles;

        std::vector<Engine::Animation> animations;

        // Mr.Skeltal
        Engine::Skeleton skeleton;
    };
}

#endif //  MESH_H
