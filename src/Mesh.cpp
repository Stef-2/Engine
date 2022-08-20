#include "Mesh.h"

Engine::Mesh::Mesh()
{
	this->vertices = {};
	this->indices = {};
	this->triangles = {};
	this->vertexArrayObject = 0;
	this->vertexBufferObject = 0;
	this->elementBufferObject = 0;
	this->instanceable = false;
	this->node = {};
}

Engine::Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
{
	this->vertices = vertices;
	this->indices = indices;
	this->triangles = {};
	this->vertexArrayObject = 0;
	this->vertexBufferObject = 0;
	this->elementBufferObject = 0;
	this->instanceable = false;

	this->Setup();
}

Engine::AnimatedMesh::AnimatedMesh(std::vector<Vertex> vertices, std::vector<AnimatedVertexExtension> vertexExtension, std::vector<unsigned int> indices, Engine::Skeleton skeleton, std::vector<Engine::Animation> animations)
{
	this->vertices = vertices;
	this->vertexExtensions = vertexExtension;
	this->indices = indices;
	this->skeleton = skeleton;
	this->animations = animations;
	this->triangles = {};
	this->vertexArrayObject = 0;
	this->vertexBufferObject = 0;
	this->elementBufferObject = 0;
	this->instanceable = false;

	this->Setup();
	this->AddAnimatedVertexExtension();
}

unsigned int Engine::Mesh::GetVertexArrayBuffer() const
{
	return this->vertexArrayObject;
}

unsigned int Engine::Mesh::GetVertexBufferObject() const
{
	return this->vertexBufferObject;
}

unsigned int Engine::Mesh::GetElementBufferObject() const
{
	return this->elementBufferObject;
}

unsigned int Engine::Mesh::GetInstancedVertexBufferObject() const
{
	return InstancedVertexBufferObject;
}

std::vector<Engine::Vertex>& Engine::Mesh::GetVertices()
{
	return this->vertices;
}

Engine::Skeleton& Engine::AnimatedMesh::GetSkeleton()
{
	return this->skeleton;
}

unsigned int Engine::AnimatedMesh::GetAnimatedVertexBufferObject()
{
	return this->animatedVertexBufferObject;
}

void Engine::AnimatedMesh::SetVertices(std::vector<AnimatedVertexExtension> vertices)
{
	this->vertexExtensions = vertices;
}

std::vector<unsigned int>& Engine::Mesh::GetIndices()
{
	return this->indices;
}

std::vector<Engine::Triangle>& Engine::Mesh::GetTriangles()
{
	return this->triangles;
}

std::vector<Engine::Animation>& Engine::AnimatedMesh::GetAnimations()
{
	return this->animations;
}

std::vector<Engine::AnimatedVertexExtension>& Engine::AnimatedMesh::GetAnimatedVertexExtensions()
{
	return this->vertexExtensions;
}

Engine::Material& Engine::Mesh::GetMaterial()
{
	return *this->material;
}

Engine::BoundingBox& Engine::Mesh::GetBoundingBox()
{
	return this->boundingBox;
}

void Engine::Mesh::SetMaterial(const Engine::Material& material)
{
	this->material = std::make_shared<Engine::Material>(material);
}

void Engine::Mesh::SetBoundingBox(glm::vec3 mins, glm::vec3 maxs)
{
	this->boundingBox = Engine::BoundingBox(mins, maxs);
}

bool Engine::Mesh::GetInstanceable() const
{
	return instanceable;
}

void Engine::Mesh::SetInstanceable(bool value, Engine::InstanceType type)
{
	// if its already set then just return
	if (value == this->instanceable && this->instanceType == type)
		return;

	// set instanceable
	if (value)
	{
		constexpr auto mat4quarter = sizeof(glm::vec4);
		constexpr auto vec3Size = sizeof(glm::vec3);

		// bind the old VAO
		glBindVertexArray(this->vertexArrayObject);

		// generate a new VBO for istancing data
		glGenBuffers(1, &this->InstancedVertexBufferObject);
		glBindBuffer(GL_ARRAY_BUFFER, this->InstancedVertexBufferObject);

		// set complex instance
		if (type == Engine::InstanceType::COMPLEX_INSTANCE)
		{
			glBufferData(GL_ARRAY_BUFFER, this->complexInstances.size() * mat4quarter * 4, &complexInstances[0], GL_STATIC_DRAW);

			glEnableVertexAttribArray(7);
			glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, 4 * mat4quarter, (void*)(0 * mat4quarter));
			glEnableVertexAttribArray(8);
			glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, 4 * mat4quarter, (void*)(1 * mat4quarter));
			glEnableVertexAttribArray(9);
			glVertexAttribPointer(9, 4, GL_FLOAT, GL_FALSE, 4 * mat4quarter, (void*)(2 * mat4quarter));
			glEnableVertexAttribArray(10);
			glVertexAttribPointer(10, 4, GL_FLOAT, GL_FALSE, 4 * mat4quarter, (void*)(3 * mat4quarter));

			glDisableVertexAttribArray(11);

			glVertexAttribDivisor(7, 1);
			glVertexAttribDivisor(8, 1);
			glVertexAttribDivisor(9, 1);
			glVertexAttribDivisor(10, 1);
		}
		// set simple instance
		else
		{
			glBufferData(GL_ARRAY_BUFFER, this->simpleInstances.size() * vec3Size, &simpleInstances[0], GL_STATIC_DRAW);

			glEnableVertexAttribArray(11);
			glVertexAttribPointer(11, 3, GL_FLOAT, GL_FALSE, vec3Size, (void*)(0 * vec3Size));

			glDisableVertexAttribArray(7);
			glDisableVertexAttribArray(8);
			glDisableVertexAttribArray(9);
			glDisableVertexAttribArray(10);

			glVertexAttribDivisor(11, 1);
		}

		// undbind VAO
		glBindVertexArray(0);
	}
	else
	{
		// bind the old VAO
		glBindVertexArray(this->vertexArrayObject);

		glDeleteBuffers(1, &this->InstancedVertexBufferObject);

		glDisableVertexAttribArray(7);
		glDisableVertexAttribArray(8);
		glDisableVertexAttribArray(9);
		glDisableVertexAttribArray(10);
		glDisableVertexAttribArray(11);

		// undbind VAO
		glBindVertexArray(0);
	}

	instanceable = value;
	this->instanceType = type;
}

template<>
void Engine::Mesh::SetInstances(std::vector<Engine::SimpleInstance>& value)
{
	this->simpleInstances = value;
}

template<>
void Engine::Mesh::SetInstances(std::vector<Engine::ComplexInstance>& value)
{
	this->complexInstances = value;
}

template<>
void Engine::Mesh::AddInstance(const Engine::SimpleInstance& instance)
{
	this->simpleInstances.push_back(instance);
}

Engine::InstanceType Engine::Mesh::GetInstanceType() const
{
	return instanceType;
}

void Engine::Mesh::SetInstanceType(Engine::InstanceType value)
{
	instanceType = value;
}

template<>
void Engine::Mesh::AddInstance(const Engine::ComplexInstance& instance)
{
	this->complexInstances.push_back(instance);
}

template<>
std::vector<Engine::SimpleInstance>& Engine::Mesh::GetInstances()
{
	return this->simpleInstances;
}

template<>
std::vector<Engine::ComplexInstance>& Engine::Mesh::GetInstances()
{
	return this->complexInstances;
}

void Engine::Mesh::SetVertices(std::vector<Engine::Vertex> vertices)
{
	this->vertices = vertices;
}

void Engine::AnimatedMesh::SetSkeleton(Engine::Skeleton& skelly)
{
	this->skeleton = skelly;
}

void Engine::Mesh::SetIndices(std::vector<unsigned int> indices)
{
	this->indices = indices;
}

void Engine::AnimatedMesh::AddAnimation(Engine::Animation animation)
{
	this->animations.push_back(animation);
}


void Engine::Mesh::Setup()
{
	// generate the buffers
	glGenVertexArrays(1, &this->vertexArrayObject);

	glGenBuffers(1, &this->vertexBufferObject);
	glGenBuffers(1, &this->elementBufferObject);

	glBindVertexArray(this->vertexArrayObject);

	// bind the element buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);
	// fill the element buffer with data
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
	// bind the vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, this->vertexBufferObject);
	// fill the vertex buffer with data
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	// setup vertex attributes

	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

	// vertex bitangents
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));

	// vertex tangents
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));

	// vertex texture coords (UVs)
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

	// unbind VAO
	glBindVertexArray(0);

	// construct triangle data out of vertices and indices
	//for (size_t i = 0; i < this->indices.size() - 2; i += 3)
		//this->triangles.push_back({ &this->vertices.at(indices.at(i)), &this->vertices.at(indices.at(i + 1)), &this->vertices.at(indices.at(i + 2)) });
}

void Engine::AnimatedMesh::AddAnimatedVertexExtension()
{
	// bind the old VAO
	glBindVertexArray(this->vertexArrayObject);

	// generate a new VBO, bind and fill with data
	glGenBuffers(1, &this->animatedVertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, this->animatedVertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, this->vertexExtensions.size() * sizeof(Engine::AnimatedVertexExtension), &this->vertexExtensions[0], GL_STATIC_DRAW);
	
	// vertex bone IDs
	glEnableVertexAttribArray(5);
	glVertexAttribIPointer(5, 4, GL_INT, sizeof(AnimatedVertexExtension), (void*)offsetof(AnimatedVertexExtension, boneID));

	// vertex bone weights
	glEnableVertexAttribArray(6);
	
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(AnimatedVertexExtension), (void*)offsetof(AnimatedVertexExtension, boneWeight));
	
	// undbind VAO
	glBindVertexArray(0);
	/*
	// construct triangle data out of vertices and indices
	#pragma omp simd
	
	for (size_t i = 0; i < this->indices.size() - 2; i += 3)
		this->triangles.emplace_back(this->vertices.at(indices.at(i)), this->vertices.at(indices.at(i + 1)), this->vertices.at(indices.at(i + 2)));
		*/
}

Engine::Triangle& Engine::Triangle::operator=(const Engine::Triangle& other)
{
	if (this != &other)
	{
		this->a = other.a;
		this->b = other.b;
		this->c = other.c;
	}
	return *this;
}

bool Engine::Triangle::operator==(const Engine::Triangle& other)
{
	return  &this->a == &other.a &&
			&this->b == &other.b &&
			&this->c == &other.c;
}
