#include "Terrain.h"

Engine::Terrain::Terrain()
{
	this->size = {};
	this->density = {};
	this->mesh = {};
	this->shader = {};
}

Engine::Terrain::Terrain(glm::dvec2 size, double density)
{
	this->size = size;
	this->density = density;
	this->mesh = {};
	this->shader = {};

	this->Generate();
}

Engine::Terrain::Terrain(glm::dvec2 size, double density, std::string hightMapPath)
{
	this->size = size;
	this->density = density;
	this->mesh = {};
	this->shader = {};

	this->SetHeightMap(hightMapPath);
	this->Generate();
}

Engine::Terrain::Terrain(glm::dvec2 size, double density, glm::vec3 position)
{
	this->size = size;
	this->density = density;
	this->position = position;
	this->mesh = {};
	this->shader = {};

	this->Generate();
}

Engine::Terrain::Terrain(glm::dvec2 size, double density, glm::vec3 position, std::string hightMapPath)
{
	this->size = size;
	this->density = density;
	this->position = position;
	this->mesh = {};
	this->shader = {};

	this->SetHeightMap(hightMapPath);
	this->Generate();
}

Engine::Mesh& Engine::Terrain::GetMesh()
{
	return this->mesh;
}

Engine::Shader& Engine::Terrain::GetShader()
{
	return this->shader;
}

Engine::Texture& Engine::Terrain::GetHeightMap()
{
	return this->heightMap;
}

glm::dvec2& Engine::Terrain::GetSize()
{
	return this->size;
}

double& Engine::Terrain::GetDensity()
{
	return this->density;
}

void Engine::Terrain::SetShader(const Engine::Shader& shader)
{
	this->shader = shader;
}

void Engine::Terrain::SetHeightMap(std::string hightMapPath)
{
	// utility var to be passed to the loader since stbi_load() doesn't like to work with nullpointers
	int temp;
	// load the heightmap texture the regular way
	this->heightMap = Engine::Texture(hightMapPath);

	// normally the texture loading process cleans up its data after its been bound and uploaded to the GPU
	// but in this case we want to keep it available for sampling as we're planning to use it for terrain generation
	// so this little hack just shoves the data back in so we can access it later
	// we're also making sure that the resulting data has 3 components / channels per pixel
	this->heightMap.SetData(stbi_load(hightMapPath.c_str(), &temp, &temp, &temp, 3));
}

void Engine::Terrain::SetSize(glm::dvec2 size)
{
	this->size = size;
}

void Engine::Terrain::SetDensity(double density)
{
	this->density = density;
}

void Engine::Terrain::ParseHeightMap()
{

}

void Engine::Terrain::Generate()
{
	// minimum and maximum terrain coordinates
	glm::dvec3 mins;
	glm::dvec3 maxs;

	// calculate terrain boundries
	mins.x = this->position.x - this->size.x / 2;
	mins.y = this->position.y;
	mins.z = this->position.z - this->size.y / 2;
	
	maxs.x = this->position.x + this->size.x / 2;
	maxs.y = this->position.y;
	maxs.z = this->position.z + this->size.y / 2;

	// calculate the vertex offsets
	double offsetX = this->size.x / (this->size.x * this->density);
	double offsetY = this->size.y / (this->size.y * this->density);

	// calculate the number of vertices in both directions
	unsigned int numVerticesX = unsigned int(this->size.x * this->density + 1);
	unsigned int numVerticesY = unsigned int(this->size.y * this->density + 1);

	// utility lambda that samples the heightmap data at a given point
	auto HeightMapSample = [this](unsigned int x, unsigned int y) -> double
	{
		unsigned char* pixelOffset = this->heightMap.GetData() + (x + this->heightMap.GetWidth() * y) * 3u;
		unsigned char red = pixelOffset[0];
		unsigned char green = pixelOffset[1];
		unsigned char blue = pixelOffset[2];

		// find the geometric mean of differences between heightmap and terrain width and length
		double scalingFactor = (((this->heightMap.GetWidth() - this->size.x) + (this->heightMap.GetHeight() - this->size.y)) / 2.0) / 256.0;

		return double((unsigned(red) + unsigned(green) + unsigned(blue)) / (3.0 + scalingFactor));
	};
	
	// stack of vertices
	std::vector<Engine::Vertex> vertices;
	// order in which they connect to form mesh surface
	std::vector<unsigned int> indices;
	// a map between a vertex and a stack of vectors that will be used as weights for its final, post transform normal calcualtions
	std::map<Engine::Vertex*, std::vector<glm::vec3>> vertexNormalWeights;
	
	// calculate, add and push vertex attributes
	for (size_t i = 0; i < numVerticesX; i++)
		for (size_t j = 0; j < numVerticesY; j++)

			// construct and add vertices to the stack
			vertices.push_back(Engine::Vertex{
				// position X
				{mins.x + offsetX * i,
				// position Y
				HeightMapSample(i * (this->heightMap.GetWidth() / numVerticesX), j * (this->heightMap.GetHeight() / numVerticesY)),
				// position Z
				mins.z + offsetY * j },
				// normal
				{ 0.0f, 1.0f, 0.0f},
				// bitangent
				{0.0f, 0.0f, 1.0f},
				// tangent
				{1.0f, 0.0f, 0.0f},
				// uv, needs to be in [0,1] range
				{(0 + offsetX * i) / this->size.x, (0 + offsetY * j) / this->size.y} });

	// initialize a connection between a vertex and its stack of normal weights
	// this could not have been done in the original loop because we're using pointers for performance reasons
	// vectors work by moving themselves around when they can no longer continuously fit into the memory hole they reside in
	// thus they relocate to a new location but our pointers keep pointing to their old addess, causing memory corruption
	// and so this step must be done only after all the vertices have been made and pushed into the vector
	for (size_t i = 0; i < vertices.size(); i++)
		// add the vertex as a map entry
		vertexNormalWeights.insert(std::pair<Engine::Vertex*, std::vector<glm::vec3>> {&vertices.at(i), std::vector<glm::vec3>{}});
		
	// terrain triangle strip creation logic
	// this is outdated from back when I wanted to do triangle strips
	// spent too much time on this ascii thing to delete it, keeping it for old times sake
	
	// --> o-o-o-o-o-o-o-o-o-o-o --+	<-vertices
	//	   |/|/|/|/|/|/|/|/|/|/|   |	<-alternating weave pattern alows one continuous strip
	// +-- o-o-o-o-o-o-o-o-o-o-o <-+	
	// |   |\|\|\|\|\|\|\|\|\|\|		<-flipping of Y direction sign produces zig zagging
	// +-> o-o-o-o-o-o-o-o-o-o-o --+	<-X direction is flipped any time we reach far left or right
	//	   |/|/|/|/|/|/|/|/|/|/|   |
	// +-- o-o-o-o-o-o-o-o-o-o-o <-+
	// |   |\|\|\|\|\|\|\|\|\|\|
	// +-> o-o-o-o-o-o-o-o-o-o-o -->	<-sick ascii thing check out my deviantArt
	
	// calculate indices for indexed rendering later on
	for (unsigned int i = 0; i < numVerticesX * numVerticesY; i++)
	{
		unsigned int quadIndexTL = i + numVerticesX;
		unsigned int quadIndexTR = i + numVerticesX + 1;
		unsigned int quadIndexBL = i;
		unsigned int quadIndexBR = i + 1;

		indices.insert(indices.end(), { quadIndexBL, quadIndexBR, quadIndexTR,
										quadIndexBL, quadIndexTR, quadIndexTL });

		if ((quadIndexBR + 1) % numVerticesX == 0) {
			i += 1;

			if (i + numVerticesX + 1 >= numVerticesX * numVerticesY)
				break;
		}
	}
	
	// calculate normals, tangents and bitangents after vertices have been displaced
	// part 1: calculate and collect normal weights for all vertices from their surrounding triangles
	for (size_t i = 0; i < indices.size(); i += 3)
	{
		Engine::Vertex& vertex1 = vertices.at(indices.at(i));
		Engine::Vertex& vertex2 = vertices.at(indices.at(i + 1));
		Engine::Vertex& vertex3 = vertices.at(indices.at(i + 2));

		// calculate normalized triangle normal
		glm::vec3 triangleNormal = glm::normalize(glm::cross(glm::vec3{ vertex2.position - vertex1.position }, glm::vec3{ vertex3.position - vertex1.position }));

		// calculate angles between the three vertices
		float angle1 = glm::angle(glm::vec3{ vertex2.position - vertex1.position }, glm::vec3{ vertex3.position - vertex1.position });
		float angle2 = glm::angle(glm::vec3{ vertex3.position - vertex2.position }, glm::vec3{ vertex1.position - vertex2.position });
		float angle3 = glm::angle(glm::vec3{ vertex1.position - vertex3.position }, glm::vec3{ vertex2.position - vertex3.position });

		// add normal weights to each vertices stack
		vertexNormalWeights.at(&vertex1).push_back(triangleNormal * angle1);
		vertexNormalWeights.at(&vertex2).push_back(triangleNormal * angle2);
		vertexNormalWeights.at(&vertex3).push_back(triangleNormal * angle3);
	}
	
	// part 2: parse the weights and calculate the final normals
	for (size_t i = 0; i < vertices.size(); i++)
	{
		glm::vec3 normal = glm::vec3(0.0f);

		for (size_t j = 0; j < vertexNormalWeights.at(&vertices.at(i)).size(); j++)
			normal += vertexNormalWeights.at(&vertices.at(i)).at(j);
		
		vertices.at(i).normal = glm::normalize(normal);
	}

	// part 3: go through all triangles once again with new normal data and calculate tangents and bitangents
	for (size_t i = 0; i < indices.size(); i += 3)
	{
		Engine::Vertex& vertex1 = vertices.at(indices.at(i));
		Engine::Vertex& vertex2 = vertices.at(indices.at(i + 1));
		Engine::Vertex& vertex3 = vertices.at(indices.at(i + 2));

		// vertex 1
		glm::vec3 posDelta2x1 = vertex2.position - vertex1.position;
		glm::vec3 posDelta3x1 = vertex3.position - vertex1.position;

		float uvDelta2x1 = vertex2.uv.y - vertex1.uv.y;
		float uvDelta3x1 = vertex2.uv.y - vertex1.uv.y;

		vertex1.tangent = glm::normalize(posDelta2x1 * uvDelta3x1 - posDelta3x1 * uvDelta2x1);
		vertex1.bitangent = glm::normalize(glm::cross(vertex1.normal, vertex1.tangent));

		// vertex 2
		glm::vec3 posDelta1x2 = vertex1.position - vertex2.position;
		glm::vec3 posDelta3x2 = vertex3.position - vertex2.position;

		float uvDelta1x2 = vertex1.uv.y - vertex2.uv.y;
		float uvDelta3x2 = vertex3.uv.y - vertex2.uv.y;

		vertex2.tangent = glm::normalize(posDelta1x2 * uvDelta3x2 - posDelta3x2 * uvDelta1x2);
		vertex2.bitangent = glm::normalize(glm::cross(vertex2.normal, vertex2.tangent));

		// vertex 3
		glm::vec3 posDelta1x3 = vertex1.position - vertex3.position;
		glm::vec3 posDelta2x3 = vertex2.position - vertex3.position;

		float uvDelta1x3 = vertex1.uv.y - vertex3.uv.y;
		float uvDelta2x3 = vertex2.uv.y - vertex3.uv.y;

		vertex3.tangent = glm::normalize(posDelta1x3 * uvDelta2x3 - posDelta2x3 * uvDelta1x3);
		vertex3.bitangent = glm::normalize(glm::cross(vertex3.normal, vertex3.tangent));
	}
	
	// push vertices and indices into the mesh and have it generate its internal rendering data
	this->mesh.SetVertices(vertices);
	this->mesh.SetIndices(indices);

	this->mesh.Setup();
}
