#ifndef TERRAIN_H
#define TERRAIN_H

#ifndef STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_IMPLEMENTATION
#endif

#include "Object.h"
#include "Texture2D.h"
#include "Shader.h"
#include "Mesh.h"

#include "glm/glm.hpp"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/gtx/vector_angle.hpp"
#include "glm/gtc/noise.hpp"
#include "glm/gtc/random.hpp"
#include "glm/gtx/common.hpp"

#include "functional"
#include <cassert>
#include "assert.h"
#include "string"
#include "array"
#include "vector"
#include "map"

namespace Engine
{

    // a chunk of terrain data
    // splitting it so we can benefit from culling and streaming optimizations
    struct Sector
    {
        std::vector<Engine::Vertex> vertices;
        std::vector<unsigned int> indices;

        // bounding box for frustum culling
        Engine::BoundingBox boundingBox;

        // openGL buffers
        unsigned int VAO;
        unsigned int VBO;
        unsigned int EBO;

        // terrain divisor if its size is greater than this value
        // competition to see how many specifiers we can put before the actual value
        inline static constexpr const unsigned short sectorSize = 16u;
    };

    // Terrain
    class Terrain : public Object
    {
        using Engine::Object::Object;

    public:
        Terrain();
        Terrain(glm::dvec2 size, double density);
        Terrain(glm::dvec2 size, double density, std::string hightMapPath);
        Terrain(glm::dvec2 size, double density, glm::vec3 position);
        Terrain(glm::dvec2 size, double density, glm::vec3 position, std::string hightMapPath);

        Engine::Mesh& GetMesh();
        Engine::ShaderProgram& GetShader();
        Engine::Texture2D& GetHeightMap();
        std::vector<Engine::Material>& GetMaterials();

        glm::dvec2& GetSize();
        double& GetDensity();

        void SetShader(const Engine::ShaderProgram& shader);
        void SetHeightMap(std::string hightMapPath);

        void SetSize(glm::dvec2 size);
        void SetDensity(double density);
        void SetSectors(std::vector<Engine::Sector> sectors);
        void SetBoundingBox(glm::vec3 mins, glm::vec3 maxs);

        void AddMaterial(Engine::Material& material);
        void AddSector(Engine::Sector& sector);

    private:
        // parses the stored height map
        void ParseHeightMap();
        // generate the terrain mesh based on our current properties
        void Generate();

        std::shared_ptr<Engine::Mesh> mesh;
        std::shared_ptr<Engine::Texture2D> heightMap;
        std::shared_ptr<Engine::ShaderProgram> shader;
        std::shared_ptr<std::vector<std::shared_ptr<Engine::Material>>> materials;

        Engine::BoundingBox boundingBox;
        std::vector<Engine::Sector> sectors;

        glm::dvec2 size;
        double density;
    };

}

#endif // TERRAIN_H