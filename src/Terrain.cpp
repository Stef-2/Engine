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

std::vector<Engine::Material>& Engine::Terrain::GetMaterials()
{
	return this->materials;
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
	this->heightMap.SetData(stbi_load(hightMapPath.c_str(), &temp, &temp, &temp, 1));
}

void Engine::Terrain::SetSize(glm::dvec2 size)
{
	this->size = size;
}

void Engine::Terrain::SetDensity(double density)
{
	this->density = density;
}

void Engine::Terrain::SetSectors(std::vector<Engine::Sector> sectors)
{
	this->sectors = sectors;
}

void Engine::Terrain::SetBoundingBox(glm::vec3 mins, glm::vec3 maxs)
{
	this->boundingBox = { mins, maxs };
}

void Engine::Terrain::AddMaterial(Engine::Material& material)
{
	this->materials.push_back(material);
}

void Engine::Terrain::AddSector(Engine::Sector& sector)
{
	this->sectors.push_back(sector);
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

	// tessellation happens on a triangle if [RateOfTriangleHeightChange] returns greater than this value
	constexpr float tessellationThreshold = 2.0f;
	// value determining the proximity of [reduceTessellate]'s inner triangle to the center of the original
	// default of 2.0f = middle, greater values bring it close to the center
	constexpr float reductionTessellationDivisor = 2.0f;
	// heightmap sampler height divisor
	constexpr float heightFactor = 2.0f;
	// number of channels / components per heightmap pixel
	constexpr unsigned int heightMapTextureComponents = 1u;

	// minimum and maximum terrain coordinates
	glm::dvec3 mins;
	glm::dvec3 maxs;

	// calculate terrain boundries
	mins.x = this->position.x - this->size.x / 2.0;
	mins.y = this->position.y;
	mins.z = this->position.z - this->size.y / 2.0;
	
	maxs.x = this->position.x + this->size.x / 2.0;
	maxs.y = this->position.y;
	maxs.z = this->position.z + this->size.y / 2.0;

	// calculate the vertex offsets
	const double offsetX = this->size.x / (this->size.x * this->density);
	const double offsetY = this->size.y / (this->size.y * this->density);

	// calculate the number of vertices in both directions
	const unsigned int numVerticesX = unsigned int(this->size.x * this->density + 1);
	const unsigned int numVerticesY = unsigned int(this->size.y * this->density + 1);

	// find the heightmap / terrain size scaling factors to be used later
	const double scalingFactorX = this->heightMap.GetWidth() / this->size.x;
	const double scalingFactorY = this->heightMap.GetHeight() / this->size.y;
	const double heightScalingFactor = ((scalingFactorX + scalingFactorY) / 2.0) / heightFactor;

	// number of pixels we sample from the map should vary depending on its size and the size of the terrain
	unsigned short mapSamplingFactor = short(scalingFactorX + scalingFactorY) >> 1;
	// it also needs to be an odd number
	mapSamplingFactor = mapSamplingFactor % 2 == 0 ? mapSamplingFactor + 1 : mapSamplingFactor;

	// utility lambda that samples the heightmap data at a given point
	auto HeightMapSample = [this](unsigned int x, unsigned int y) -> float
	{
		unsigned char* pixel = this->heightMap.GetData() + (x + this->heightMap.GetWidth() * y);

		return float(unsigned(*pixel));
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
					unsigned char* pixel = this->heightMap.GetData() + ((x + i) + this->heightMap.GetWidth() * (y + j));

					samples[index] = float((unsigned(*pixel)));

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
	auto RateOfTriangleHeightChange = [this, &HeightMapSample, &scalingFactorX, &scalingFactorY, &heightScalingFactor]
	(const Engine::Vertex& A, const Engine::Vertex& B, const Engine::Vertex& C) -> double
	{
		// number of samples to take, relative to the size of the height map
		const unsigned int numSamples = this->heightMap.GetWidth() * this->heightMap.GetHeight() / ((this->heightMap.GetWidth() + this->heightMap.GetHeight()) * 32u);
		// accumulated height difference
		float heightDifference = 0.0f;

		// calculate triangle normal
		glm::vec3 normal = glm::normalize((A.normal + B.normal + C.normal) / 3.0f);

		// take an initial reference sample
		float averageTriangleHeight = (A.position.y + B.position.y + C.position.y) / 3.0f;

		// go through samples and accumulate the difference in height
		//#pragma omp parallel for
		for (long i = 0; i < numSamples; i++)
		{
			// take a pair of random (0.0f, 1.0f) samples
			glm::vec2 randomSample = { glm::linearRand(0.0f, 1.0f), glm::linearRand(0.0f, 1.0f) };

			// square root of the first random number, its used a lot afterwards
			float root = glm::sqrt(randomSample.x);

			// use the random numbers to find a sample withing a triangle
			glm::vec3 randomPoint = A.position * (1 - root) + B.position * (root * (1 - randomSample.y)) + C.position * (randomSample.y * root);

			// get the difference between the average triangle height and the sampled point height, add it to the total
			heightDifference += glm::abs(averageTriangleHeight - 
										(HeightMapSample((randomPoint.x + this->size.x / 2) * scalingFactorX,
														 (randomPoint.z + this->size.y / 2) * scalingFactorY)) / heightScalingFactor);
		}

		// dividing the accumulated height by the number of samples gives us the average height deviation
		return heightDifference / numSamples;
	};
	
	// utility lambdas that will calculate a centroid or an incircle of a triangle, either can be used for the following tessellation algorithms
	auto centroid = [this, &HeightMapSample, &heightScalingFactor, &scalingFactorX, &scalingFactorY]
					(Engine::Vertex& A, Engine::Vertex& B, Engine::Vertex& C) -> Engine::Vertex
	{
		float positionX = (A.position.x + B.position.x + C.position.x) / 3.0f;
		float positionZ = (A.position.z + B.position.z + C.position.z) / 3.0f;

		// sample the heightmap for the Y coordinate
		float positionY = HeightMapSample((positionX * scalingFactorX + this->heightMap.GetWidth() / 2),
			(positionZ * scalingFactorY + this->heightMap.GetHeight() / 2)) / heightScalingFactor;

		float uvX = (A.uv.x + B.uv.x + C.uv.x) / 3.0f;
		float uvY = (A.uv.y + B.uv.y + C.uv.y) / 3.0f;

		return Engine::Vertex{
			// position
			{positionX, positionY, positionZ},
			// normal
			{ 0.0f, 1.0f, 0.0f},
			// bitangent
			{0.0f, 0.0f, 1.0f},
			// tangent
			{1.0f, 0.0f, 0.0f},
			// uv, needs to be in [0,1] range
			{uvX, uvY } };;
	};

	auto incircle = [this, &HeightMapSample, &heightScalingFactor, &scalingFactorX, &scalingFactorY]
					(Engine::Vertex& A, Engine::Vertex& B, Engine::Vertex& C) -> Engine::Vertex
	{
		float sideA = glm::distance(B.position, C.position);
		float sideB = glm::distance(A.position, C.position);
		float sideC = glm::distance(A.position, B.position);

		float positionX = (A.position.x * sideA + B.position.x * sideB + C.position.x * sideC) / (sideA + sideB + sideC);
		float positionZ = (A.position.z * sideA + B.position.z * sideB + C.position.z * sideC) / (sideA + sideB + sideC);

		// sample the heightmap for the Y coordinate
		float positionY = HeightMapSample((positionX * scalingFactorX + this->heightMap.GetWidth() / 2),
			(positionZ * scalingFactorY + this->heightMap.GetHeight() / 2)) / heightScalingFactor;

		sideA = glm::distance(B.uv, C.uv);
		sideB = glm::distance(A.uv, C.uv);
		sideC = glm::distance(A.uv, B.uv);

		float uvX = (A.uv.x * sideA + B.uv.x * sideB + C.uv.x * sideC) / (sideA + sideB + sideC);
		float uvY = (A.uv.y * sideA + B.uv.y * sideB + C.uv.y * sideC) / (sideA + sideB + sideC);

		return Engine::Vertex{
			// position
			{positionX, positionY, positionZ},
			// normal
			{ 0.0f, 1.0f, 0.0f},
			// bitangent
			{0.0f, 0.0f, 1.0f},
			// tangent
			{1.0f, 0.0f, 0.0f},
			// uv, needs to be in [0,1] range
			{uvX, uvY } };;
	};

	// lambda that will iteratively tessellate a triangle [numIterations] number of times
	// works by creating a new vertex at the center of the triangle
	std::function<void(std::vector<Engine::Vertex>& vertices, std::vector<unsigned int>& indices, unsigned short numIterations, const unsigned int& startIndex)>
	pinchTessellate = [this, &HeightMapSample, &scalingFactorX, &scalingFactorY, &heightScalingFactor, &pinchTessellate, &centroid, &incircle]
	(std::vector<Engine::Vertex>& vertices, std::vector<unsigned int>& indices, unsigned short numIterations, const unsigned int& startIndex) -> void
	{
		if (!numIterations)
			return;

		unsigned short tessellationDepth = numIterations - 1u;

		unsigned int indexA = indices.at(size_t(startIndex) + 0);
		unsigned int indexB = indices.at(size_t(startIndex) + 1);
		unsigned int indexC = indices.at(size_t(startIndex) + 2);

		Engine::Vertex& A = vertices.at(indexA);
		Engine::Vertex& B = vertices.at(indexB);
		Engine::Vertex& C = vertices.at(indexC);

		// create a new vertex in the center of the triangle
		Engine::Vertex D = centroid(A, B, C);

		vertices.push_back(D);
		const unsigned int&& indexD = vertices.size() - 1;

		// mark old indices for deletion, deleting them now would create a mess
		indices.at(size_t(startIndex) + 0) = -1;
		indices.at(size_t(startIndex) + 1) = -1;
		indices.at(size_t(startIndex) + 2) = -1;

		// add the new ones
		indices.insert(indices.end(), { indexA, indexB, indexD,
										indexD, indexB, indexC,
										indexC, indexA, indexD });
		
		unsigned int numIndices = indices.size();

		// if we need to tessellate deeper, pass the call to the newly created triangles
		if (tessellationDepth) {
			pinchTessellate(vertices, indices, tessellationDepth, numIndices - 3u);
			pinchTessellate(vertices, indices, tessellationDepth, numIndices - 6u);
			pinchTessellate(vertices, indices, tessellationDepth, numIndices - 9u);
		}
	};

	// lambda that will iteratively tessellate a triangle [numIterations] number of times
	// works by creating a new triangle inside the old one
	std::function<void(std::vector<Engine::Vertex>& vertices, std::vector<unsigned int>& indices, unsigned short numIterations, const unsigned int& startIndex)>
		reduceTessellate = [this, &HeightMapSample, &scalingFactorX, &scalingFactorY, &heightScalingFactor, &pinchTessellate, &reduceTessellate, &centroid, &incircle]
		(std::vector<Engine::Vertex>& vertices, std::vector<unsigned int>& indices, unsigned short numIterations, const unsigned int& startIndex) -> void
	{
		if (!numIterations)
			return;

		unsigned short tessellationDepth = numIterations - 1u;

		unsigned int indexA = indices.at(size_t(startIndex) + 0);
		unsigned int indexB = indices.at(size_t(startIndex) + 1);
		unsigned int indexC = indices.at(size_t(startIndex) + 2);

		Engine::Vertex& A = vertices.at(indexA);
		Engine::Vertex& B = vertices.at(indexB);
		Engine::Vertex& C = vertices.at(indexC);

		Engine::Vertex center = centroid(A, B, C);

		Engine::Vertex A1{
			// position
			(A.position + center.position * (reductionTessellationDivisor - 1)) / reductionTessellationDivisor,
			// normal
			{ 0.0f, 1.0f, 0.0f},
			// bitangent
			{0.0f, 0.0f, 1.0f},
			// tangent
			{1.0f, 0.0f, 0.0f},
			// uv, needs to be in [0,1] range
			(A.uv + center.uv * (reductionTessellationDivisor - 1)) / reductionTessellationDivisor };

		Engine::Vertex B1{
			// position
			(B.position + center.position * (reductionTessellationDivisor - 1)) / reductionTessellationDivisor,
			// normal
			{ 0.0f, 1.0f, 0.0f},
			// bitangent
			{0.0f, 0.0f, 1.0f},
			// tangent
			{1.0f, 0.0f, 0.0f},
			// uv, needs to be in [0,1] range
			(B.uv + center.uv * (reductionTessellationDivisor - 1)) / reductionTessellationDivisor };

		Engine::Vertex C1{
			// position
			(C.position + center.position * (reductionTessellationDivisor - 1)) / reductionTessellationDivisor,
			// normal
			{ 0.0f, 1.0f, 0.0f},
			// bitangent
			{0.0f, 0.0f, 1.0f},
			// tangent
			{1.0f, 0.0f, 0.0f},
			// uv, needs to be in [0,1] range
			(C.uv + center.uv * (reductionTessellationDivisor - 1)) / reductionTessellationDivisor };

		// add the new vertices
		vertices.insert(vertices.end(), { A1, B1, C1 });

		const unsigned int&& indexA1 = vertices.size() - 3;
		const unsigned int&& indexB1 = vertices.size() - 2;
		const unsigned int&& indexC1 = vertices.size() - 1;

		// mark old indices for deletion, deleting them now would create a mess
		indices.at(size_t(startIndex) + 0) = -1;
		indices.at(size_t(startIndex) + 1) = -1;
		indices.at(size_t(startIndex) + 2) = -1;

		// add the new ones
		indices.insert(indices.end(), {indexA,  indexB,  indexB1,
									   indexA,  indexB1, indexA1,
									   indexB,  indexC,  indexC1,
									   indexB,  indexC1, indexB1,
									   indexC,  indexA,  indexA1,
									   indexC,  indexA1, indexC1,
									   indexA1, indexB1, indexC1} );

		// if we need to tessellate deeper, pass the call to the newly created center triangle
		// keep using reduction tessellation for depths above 1
		if (tessellationDepth > 1)
			reduceTessellate(vertices, indices, tessellationDepth, indices.size() - 3u);
		
		// for depth 1, just pinch tesselate the center triangle and its done
		else if (tessellationDepth == 1)
			pinchTessellate(vertices, indices, tessellationDepth, indices.size() - 3u);

		
	};

	// stack of vertices to fill in
	std::vector<Engine::Vertex> vertices;
	// order in which they connect to form mesh surface
	std::vector<unsigned int> indices;
	// a map between a vertex and a stack of vectors that will be used as weights for its final, post transform normal calcualtions
	std::unordered_map<Engine::Vertex*, std::vector<glm::vec3>> vertexNormalWeights;

	// reserve vector space so we don't cause 6 million relocations later
	vertices.reserve(size_t(numVerticesX) * numVerticesY);
	indices.reserve(size_t(numVerticesX) * numVerticesY * 3);
	vertexNormalWeights.reserve(size_t(numVerticesX) * numVerticesY);
	
	// calculate, add and push vertex attributes
	for (long long i = 0; i < numVerticesX; i++)
		#pragma omp parallel for shared(vertices) ordered
		for (long long j = 0; j < numVerticesY; j++)
		{
			// construct and add vertices to the stack
			#pragma omp ordered 
			{
				vertices.push_back(Engine::Vertex{
					// position X
					{mins.x + offsetX * i,
					// position Y, middle of the 3x3 sample
					0.0f,
					//HeightMapSample(i * this->heightMap.GetWidth() / numVerticesX , j * this->heightMap.GetHeight() / numVerticesY) / heightScalingFactor,
					// position Z
					mins.z + offsetY * j },
					// normal
					{0.0f, 1.0f, 0.0f},
					// bitangent
					{0.0f, 0.0f, 1.0f},
					// tangent
					{1.0f, 0.0f, 0.0f},
					// uv, needs to be in [0,1] range
					{(offsetX * i) / this->size.x, (offsetY * j) / this->size.y} });

				vertices.back().position.y = HeightMapSample((vertices.back().position.x * scalingFactorX + this->heightMap.GetWidth() / 2),
					(vertices.back().position.z * scalingFactorY + this->heightMap.GetHeight() / 2)) / heightScalingFactor;
			}
		}
	
	// terrain triangle strip creation logic
	// this is outdated from back when I wanted to do triangle strips for terrain mesh
	// spent too much time on this ascii thing to delete it, keeping it for old times sake

	// --> o-o-o-o-o-o-o-o-o-o-o --+	<-vertices
	//	   |/|/|/|/|/|/|/|/|/|/|   |	<-alternating weave pattern allows one continuous strip
	// +-- o-o-o-o-o-o-o-o-o-o-o <-+	
	// |   |\|\|\|\|\|\|\|\|\|\|		<-flipping of Y direction sign produces zig zagging
	// +-> o-o-o-o-o-o-o-o-o-o-o --+	<-X direction is flipped any time we reach far left or right
	//	   |/|/|/|/|/|/|/|/|/|/|   |
	// +-- o-o-o-o-o-o-o-o-o-o-o <-+
	// |   |\|\|\|\|\|\|\|\|\|\|
	// +-> o-o-o-o-o-o-o-o-o-o-o -->	<-sick ascii thing check out my DeviantArt

	// calculate indices for indexed rendering later on
	for (signed long long i = 0; i < signed long long(numVerticesX) * numVerticesY; i++)
	{
		unsigned int quadIndexBL = i;
		unsigned int quadIndexBR = i + 1;
		unsigned int quadIndexTL = i + numVerticesX;
		unsigned int quadIndexTR = i + numVerticesX + 1;

		indices.insert(indices.end(), { quadIndexBL, quadIndexBR, quadIndexTR,
										quadIndexBL, quadIndexTR, quadIndexTL });

		if ((quadIndexBR + 1) % numVerticesX == 0) {
			i += 1;

			if (i + numVerticesX + 1 >= numVerticesX * numVerticesY)
				break;
		}
	}

	unsigned long long numIndices = indices.size();
	double max = 0, min = 0;
	// tessellate triangles
	#pragma omp parallel for shared(indices, vertices) ordered
	for (signed long long i = 0; i < numIndices; i += 3)
	{
		Engine::Vertex& vertex1 = vertices.at(indices.at(i + 0));
		Engine::Vertex& vertex2 = vertices.at(indices.at(i + 1));
		Engine::Vertex& vertex3 = vertices.at(indices.at(i + 2));

		double rate = RateOfTriangleHeightChange(vertex1, vertex2, vertex3);
		min = glm::min(min, rate);
		max = glm::max(max, rate);
		if (rate > 0.0) {
			/*vertex1.tangent = { 1.0f, 1.0f, 1.0f };
			vertex1.bitangent = { 1.0f, 1.0f, 1.0f };
			vertex2.tangent = { 1.0f, 1.0f, 1.0f };
			vertex2.bitangent = { 1.0f, 1.0f, 1.0f };
			vertex3.tangent = { 1.0f, 1.0f, 1.0f };
			vertex3.bitangent = { 1.0f, 1.0f, 1.0f };*/

			#pragma omp ordered
			pinchTessellate(vertices, indices, 1, i);
			//break;
		}
		else {/*
			vertex1.tangent = { 0.0f, 0.0f, 0.0f };
			vertex1.bitangent = { 0.0f, 0.0f, 0.0f };
			vertex2.tangent = { 0.0f, 0.0f, 0.0f };
			vertex2.bitangent = { 0.0f, 0.0f, 0.0f };
			vertex3.tangent = { 0.0f, 0.0f, 0.0f };
			vertex3.bitangent = { 0.0f, 0.0f, 0.0f };*/
		}
	}

	// tessellation has marked some indices for deletion, get rid of them
	std::erase(indices, unsigned int(-1));
	std::cout << "min: " << min << ", max: " << max << std::endl;
	// initialize a connection between a vertex and its stack of normal weights
	// this could not have been done in the original loop because we're using pointers for performance reasons
	// vectors work by moving themselves around memory when they can no longer continuously fit into the memory hole they reside in
	// thus they relocate to a new location but our pointers keep pointing to their old address, causing memory corruption
	// and so this step must be done only after all the vertices have been made and pushed into the vector
	#pragma omp parallel for shared(vertices) ordered
	for (signed long long i = 0; i < vertices.size(); i++)
		// add the vertex as a map entry
		#pragma omp ordered
		vertexNormalWeights.insert(std::pair<Engine::Vertex*, std::vector<glm::vec3>> {&vertices.at(i), std::vector<glm::vec3>{}});
	
	// calculate normals, tangents and bitangents after vertices have been displaced
	// part 1: calculate and collect normal weights for all vertices from their surrounding triangles
	#pragma omp parallel for shared(indices, vertices, vertexNormalWeights) ordered
	for (signed long long i = 0; i < indices.size(); i += 3)
	{
		Engine::Vertex& vertex1 = vertices.at(indices.at(i + 0));
		Engine::Vertex& vertex2 = vertices.at(indices.at(i + 1));
		Engine::Vertex& vertex3 = vertices.at(indices.at(i + 2));

		// calculate normalized triangle normal
		glm::vec3 triangleNormal = glm::normalize(glm::cross(glm::vec3{ vertex2.position - vertex1.position }, glm::vec3{ vertex3.position - vertex1.position }));

		// calculate angles between the three vertices
		float angle1 = glm::angle(glm::vec3{ vertex2.position - vertex1.position }, glm::vec3{ vertex3.position - vertex1.position });
		float angle2 = glm::angle(glm::vec3{ vertex3.position - vertex2.position }, glm::vec3{ vertex1.position - vertex2.position });
		float angle3 = glm::angle(glm::vec3{ vertex1.position - vertex3.position }, glm::vec3{ vertex2.position - vertex3.position });
		
		// things get messed up unless we fix zero angles
		angle1 = angle1 ? angle1 : 0.1f;
		angle2 = angle2 ? angle2 : 0.1f;
		angle3 = angle3 ? angle3 : 0.1f;

		// add normal weights to each vertices stack
		#pragma omp ordered
		{
			vertexNormalWeights.at(&vertex1).push_back(triangleNormal * angle1);
			vertexNormalWeights.at(&vertex1).push_back(triangleNormal * angle2);
			vertexNormalWeights.at(&vertex1).push_back(triangleNormal * angle3);

			vertexNormalWeights.at(&vertex2).push_back(triangleNormal * angle1);
			vertexNormalWeights.at(&vertex2).push_back(triangleNormal * angle2);
			vertexNormalWeights.at(&vertex2).push_back(triangleNormal * angle3);

			vertexNormalWeights.at(&vertex3).push_back(triangleNormal * angle1);
			vertexNormalWeights.at(&vertex3).push_back(triangleNormal * angle2);
			vertexNormalWeights.at(&vertex3).push_back(triangleNormal * angle3);
		}
	}
	
	// part 2: parse the weights and calculate the final normals
	#pragma omp parallel for shared(vertices, vertexNormalWeights)
	for (signed long long i = 0; i < vertices.size(); i++)
	{
		glm::vec3 normal {0.0f, 0.0f, 0.0f};

		for (size_t j = 0; j < vertexNormalWeights.at(&vertices.at(i)).size(); j++)
			normal += vertexNormalWeights.at(&vertices.at(i)).at(j);

		vertices.at(i).normal = glm::normalize(normal);
	}
	
	// part 3: go through all triangles once again with new normal data and calculate tangents and bitangents
	#pragma omp parallel for shared(indices, vertices)
	for (signed long long i = 0; i < indices.size(); i += 3)
	{
		Engine::Vertex& vertex1 = vertices.at(indices.at(i + 0));
		Engine::Vertex& vertex2 = vertices.at(indices.at(i + 1));
		Engine::Vertex& vertex3 = vertices.at(indices.at(i + 2));

		// vertex 1
		glm::vec3 posDelta2x1 = vertex2.position - vertex1.position;
		glm::vec3 posDelta3x1 = vertex3.position - vertex1.position;

		float uvDelta2x1 = vertex2.uv.y - vertex1.uv.y;
		float uvDelta3x1 = vertex3.uv.y - vertex1.uv.y;

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
