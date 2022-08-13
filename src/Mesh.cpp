#include "Mesh.h"

Engine::Mesh::Mesh()
{
	this->vertices = {};
	this->indices = {};
	this->triangles = {};
	this->material = {};
	this->vertexArrayObject = 0;
	this->vertexBufferObject = 0;
	this->elementBufferObject = 0;
	this->node = {};
}

Engine::Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
{
	this->vertices = vertices;
	this->indices = indices;
	this->triangles = {};
	this->material = {};
	this->vertexArrayObject = 0;
	this->vertexBufferObject = 0;
	this->elementBufferObject = 0;

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
	this->material = {};
	this->vertexArrayObject = 0;
	this->vertexBufferObject = 0;
	this->elementBufferObject = 0;

	//this->Setup();
	this->AddAnimatedVertexExtension();
}

unsigned int Engine::Mesh::GetVertexArrayBuffer()
{
	return this->vertexArrayObject;
}

unsigned int Engine::Mesh::GetVertexBufferObject()
{
	return this->vertexBufferObject;
}

unsigned int Engine::Mesh::GetElementBufferObject()
{
	return this->elementBufferObject;
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

void Engine::Mesh::SetInstanceable(bool value)
{
	instanceable = value;
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
{/*
	glGenVertexArrays(1, &this->vertexArrayObject);

	glGenBuffers(1, &this->vertexBufferObject);
	glGenBuffers(1, &this->elementBufferObject);

	glBindVertexArray(this->vertexArrayObject);
	
	// bind the vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, this->vertexBufferObject);
	// fill the vertex buffer with data
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);

	// bind the element buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);
	// fill the element buffer with data
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(unsigned int), &this->indices[0], GL_STATIC_DRAW);

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

	// vertex bone IDs
	glEnableVertexAttribArray(5);
	glVertexAttribIPointer(5, 4, GL_INT, sizeof(AnimatedVertexExtension), (void*)offsetof(AnimatedVertexExtension, boneID));

	// vertex bone weights
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(AnimatedVertexExtension), (void*)offsetof(AnimatedVertexExtension, boneWeight));
	
	// unbind VAO
	glBindVertexArray(0);
	*/
	this->Setup();

	glBindVertexArray(this->vertexArrayObject);
	glGenBuffers(1, &this->animatedVertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, this->animatedVertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, this->vertexExtensions.size() * sizeof(Engine::AnimatedVertexExtension), &this->vertexExtensions[0], GL_STATIC_DRAW);
	// vertex bone IDs
	glEnableVertexAttribArray(5);
	glVertexAttribIPointer(5, 4, GL_INT, sizeof(AnimatedVertexExtension), (void*)offsetof(AnimatedVertexExtension, boneID));

	// vertex bone weights
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(AnimatedVertexExtension), (void*)offsetof(AnimatedVertexExtension, boneWeight));
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