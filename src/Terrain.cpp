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
	// check if either size or density are zero, if they are, we'll be dividing things by zero a lot
	if (!this->size.x || !this->size.y || !this->density) {
		std::cerr << "terrain generation parameters are invalid, aborting like your mother should have aborted you" << std::endl;
		return;
	}

	constexpr float tessellationThreshold = 2.0f;
	constexpr float heightFactor = 2.0f;
	constexpr unsigned int heightMapTextureComponents = 3u;

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

	// find the heightmap / terrain size scaling factors to be used later
	double scalingFactorX = this->heightMap.GetWidth() / this->size.x;
	double scalingFactorY = this->heightMap.GetHeight() / this->size.y;
	double scalingFactor = ((scalingFactorX + scalingFactorY) / 2.0) / heightFactor;

	// utility lambda that samples the heightmap data at a given point
	auto HeightMapSample = [this](unsigned int x, unsigned int y) -> float
	{
		// during texture reading we ensured that we have 3 color components / channels per pixel in case the heightmap isn't greyscale
		unsigned char* pixel = this->heightMap.GetData() + (x + this->heightMap.GetWidth() * y) * heightMapTextureComponents;

		unsigned char red = pixel[0];
		unsigned char green = pixel[1];
		unsigned char blue = pixel[2];

		return float((unsigned(red) + unsigned(green) + unsigned(blue)) / heightMapTextureComponents);
	};

	// utility lambda that samples a NxN pixel neighborhood around a given center point
	auto HeightMapNeighborhoodSample = [this](unsigned int x, unsigned int y, unsigned short neighborhoodSize) -> float*
	{
		float* samples = new float[neighborhoodSize * neighborhoodSize] {0.0f};
		unsigned short index = 0;

		for (signed short i = -(neighborhoodSize - 1) / 2; i <= (neighborhoodSize - 1) / 2; i++)
			for (signed short j = -(neighborhoodSize - 1) / 2; j <= (neighborhoodSize - 1) / 2; j++)
			{
				unsigned int offset = (x + i) + this->heightMap.GetWidth() * (y + j);

				// check if we'd sample out of bounds
				if (offset >= 0 && offset <= this->heightMap.GetWidth() * this->heightMap.GetHeight())
				{
					// during texture reading we ensured that we have 3 color components / channels per pixel in case the heightmap isn't greyscale
					unsigned char* pixel = this->heightMap.GetData() + ((x + i) + this->heightMap.GetWidth() * (y + j)) * heightMapTextureComponents;

					unsigned char red = pixel[0];
					unsigned char green = pixel[1];
					unsigned char blue = pixel[2];

					samples[index] = float((unsigned(red) + unsigned(green) + unsigned(blue)) / heightMapTextureComponents);

				}
				// if we would, just insert a perfect (0.0f, 1.0f, 0.0f) normal vector
				// this will only happen on borders and should not matter
				else
					samples[index] = 1.0f;

				index += 1u;
			}

		return samples;
	};

	// lambda that samples and calculates the rate of change of height inside a triangle
	// greater return value indicates higher relative change of contained height values
	auto RateOfTriangleHeightChange = [this, &HeightMapSample, &scalingFactorX, &scalingFactorY, &scalingFactor]
	(const glm::vec3& A, const glm::vec3& B, const glm::vec3& C, const glm::vec3& normal) -> double
	{
		// number of samples to take, relative to the size of the height map
		const unsigned int numSamples = this->heightMap.GetWidth() * this->heightMap.GetHeight() / ((this->heightMap.GetWidth() + this->heightMap.GetHeight()) * 32u);
		// accumulated height difference
		float heightDifference = 0.0f;

		// take an initial reference sample
		float averageTriangleHeight = (A.y + B.y + C.y) / 3.0f;

		// go through samples and accumulate the difference in height
		for (size_t i = 0; i < numSamples; i++)
		{
			// take a pair of random (0.0f, 1.0f) samples
			glm::vec2 randomSample = { glm::linearRand(0.0f, 1.0f), glm::linearRand(0.0f, 1.0f) };

			// square root of the first random number, its used a lot afterwards
			float root = glm::sqrt(randomSample.x);

			// use the random numbers to find a sample withing a triangle
			glm::vec3 randomPoint = A * (1 - root) + B * (root * (1 - randomSample.y)) + C * (randomSample.y * root);

			// get the difference between the average triangle height and the sampled point height, add it to the total
			heightDifference += glm::abs(averageTriangleHeight - 
				(HeightMapSample((randomPoint.x + this->size.x / 2) * scalingFactorX,
				(randomPoint.z + this->size.y / 2) * scalingFactorY)) / scalingFactor);
		}

		// dividing the accumulated height by the number of samples gives us the average height deviation
		return heightDifference / numSamples;
	};
	
	// lambda that will iteratively tessellate a triangle [numIterations] number of times
	std::function<void(std::vector<Engine::Vertex>&, std::vector<unsigned int>&, unsigned short, const unsigned int&)>

	tessellate = [this, &HeightMapSample, &scalingFactorX, &scalingFactorY, &scalingFactor, &tessellate]
	(std::vector<Engine::Vertex>& vertices, std::vector<unsigned int>& indices, unsigned short numIterations, const unsigned int& startIndex) -> void
	{
		unsigned int& indexA = indices.at(startIndex);
		unsigned int& indexB = indices.at(startIndex + 1l);
		unsigned int& indexC = indices.at(startIndex + 2l);

		Engine::Vertex& A = vertices.at(indices.at(indexA));
		Engine::Vertex& B = vertices.at(indices.at(indexB));
		Engine::Vertex& C = vertices.at(indices.at(indexC));

		float positionX = (A.position.x + B.position.x + C.position.x) / 3.0f;
		float positionZ = (A.position.z + B.position.z + C.position.z) / 3.0f;
		float positionY = HeightMapSample((positionX + this->size.x / 2) * scalingFactorX,
										  (positionZ + this->size.y / 2) * scalingFactorY) / scalingFactor;

		float uvX = (A.uv.x + B.uv.x + C.uv.x) / 2.0f;
		float uvY = (A.uv.y + B.uv.y + C.uv.y) / 2.0f;

		// create a new vertex in the center of the triangle
		Engine::Vertex D{
			// position
			{positionX, positionY, positionY},
			// normal
			{ 0.0f, 1.0f, 0.0f},
			// bitangent
			{0.0f, 0.0f, 1.0f},
			// tangent
			{1.0f, 0.0f, 0.0f},
			// uv, needs to be in [0,1] range
			{uvX, uvY } };

		vertices.push_back(D);
		const unsigned int&& indexD = vertices.size();

		// erase the old indices
		indices.erase(indices.begin() + startIndex, indices.begin() + startIndex + 2u);

		// add the new ones
		indices.insert(indices.end(), { indexA, indexB, indexD,
										indexC, indexB, indexC,
										indexC, indexA, indexD });

		// if we need to tessellate deeper, pass the call to the newly created triangles
		if (numIterations) {
			tessellate(vertices, indices, --numIterations, indices.size() - 9u);
			tessellate(vertices, indices, --numIterations, indices.size() - 6u);
			tessellate(vertices, indices, --numIterations, indices.size() - 3u);
		}
	};

	// stack of vertices to fill in
	std::vector<Engine::Vertex> vertices;
	// order in which they connect to form mesh surface
	std::vector<unsigned int> indices;
	// a map between a vertex and a stack of vectors that will be used as weights for its final, post transform normal calcualtions
	std::map<Engine::Vertex*, std::vector<glm::vec3>> vertexNormalWeights;
	
	// calculate, add and push vertex attributes
	#pragma omp simd
	for (size_t i = 0; i < numVerticesX; i++)
		for (size_t j = 0; j < numVerticesY; j++)
		{
			// sample a 3x3 pixel neighborhood around the center vertex, we need the surrounding pixel values to calculate normals, tangents and bitangnts
			float* samples = HeightMapNeighborhoodSample(i * this->heightMap.GetWidth() / numVerticesX, j * this->heightMap.GetHeight() / numVerticesY, 3u);
			// tangent is the difference between right and left sample
			glm::vec3 tangent = glm::normalize(glm::vec3{ 1.0, samples[5] - samples[3], 0.0 });
			// bitangent is the difference between bottom and top sample
			glm::vec3 bitangent = glm::normalize(glm::vec3{ 0.0, samples[7] - samples[1], 1.0 });
			// normal is just their cross product
			glm::vec3 normal = glm::normalize(glm::cross(bitangent, tangent));

			// construct and add vertices to the stack
			vertices.push_back(Engine::Vertex{
				// position X
				{mins.x + offsetX * i,
				// position Y, middle of the 3x3 sample
				samples[4] / scalingFactor,
				// position Z
				mins.z + offsetY * j },
				// normal
				//glm::normalize(glm::vec3(2 * (samples[5]-samples[3]), 4.0f, 2 * (samples[7] - samples[1]))),
				normal,
				// bitangent
				bitangent,
				// tangent
				tangent,
				// uv, needs to be in [0,1] range
				{(offsetX * i) / this->size.x, (offsetY * j) / this->size.y} });

			delete[]samples;
		}

	// initialize a connection between a vertex and its stack of normal weights
	// this could not have been done in the original loop because we're using pointers for performance reasons
	// vectors work by moving themselves around memory when they can no longer continuously fit into the memory hole they reside in
	// thus they relocate to a new location but our pointers keep pointing to their old address, causing memory corruption
	// and so this step must be done only after all the vertices have been made and pushed into the vector
	//#pragma omp parallel
	for (size_t i = 0; i < vertices.size(); i++)
		// add the vertex as a map entry
		vertexNormalWeights.insert(std::pair<Engine::Vertex*, std::vector<glm::vec3>> {&vertices.at(i), std::vector<glm::vec3>{}});
		
	// terrain triangle strip creation logic
	// this is outdated from back when I wanted to do triangle strips for terrain mesh
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
	//#pragma omp parallel
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
	/*#pragma omp simd
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
	#pragma omp simd
	for (size_t i = 0; i < vertices.size(); i++)
	{
		glm::vec3 normal = glm::vec3(0.0f);

		for (size_t j = 0; j < vertexNormalWeights.at(&vertices.at(i)).size(); j++)
			normal += vertexNormalWeights.at(&vertices.at(i)).at(j);
		
		vertices.at(i).normal = glm::normalize(normal);
	}
	
	// part 3: go through all triangles once again with new normal data and calculate tangents and bitangents
	#pragma omp simd
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

		glm::vec3 faceNormal = glm::normalize((vertex1.normal + vertex2.normal + vertex3.normal) / 3.0f);
		/*
		if (RateOfTriangleHeightChange(vertex1.position, vertex2.position, vertex3.position, faceNormal) > 1) {
			vertex1.tangent = { 1.0f, 1.0f, 1.0f };
			vertex1.bitangent = { 1.0f, 1.0f, 1.0f };
			vertex2.tangent = { 1.0f, 1.0f, 1.0f };
			vertex2.bitangent = { 1.0f, 1.0f, 1.0f };
			vertex3.tangent = { 1.0f, 1.0f, 1.0f };
			vertex3.bitangent = { 1.0f, 1.0f, 1.0f };
		}
		else {
			vertex1.tangent = { 0.0f, 0.0f, 0.0f };
			vertex1.bitangent = { 0.0f, 0.0f, 0.0f };
			vertex2.tangent = { 0.0f, 0.0f, 0.0f };
			vertex2.bitangent = { 0.0f, 0.0f, 0.0f };
			vertex3.tangent = { 0.0f, 0.0f, 0.0f };
			vertex3.bitangent = { 0.0f, 0.0f, 0.0f };
		}*/
	//}
	
	// push vertices and indices into the mesh and have it generate its internal rendering data
	this->mesh.SetVertices(vertices);
	this->mesh.SetIndices(indices);

	this->mesh.Setup();
}
