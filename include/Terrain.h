#ifndef TERRAIN_H
#define TERRAIN_H

#ifndef STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_IMPLEMENTATION
#endif

#include "Object.h"
#include "Texture.h"
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
#include "string"
#include "vector"
#include "map"

namespace Engine
{

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
        Engine::Shader& GetShader();
        Engine::Texture& GetHeightMap();

        glm::dvec2& GetSize();
        double& GetDensity();

        void SetShader(const Engine::Shader& shader);
        void SetHeightMap(std::string hightMapPath);

        void SetSize(glm::dvec2 size);
        void SetDensity(double density);

    private:
        // parses the stored height map
        void ParseHeightMap();
        // generate the terrain mesh based on our current properties
        void Generate();

        Engine::Mesh mesh;
        Engine::Shader shader;
        Engine::Texture heightMap;

        glm::dvec2 size;
        double density;
    };

}

#endif // TERRAIN_H