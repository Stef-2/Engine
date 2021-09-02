#ifndef RENDERER_H
#define RENDERER_H

// #include "glfw3.h"
#include "Camera.h"
#include "Actor.h"
#include "BoundingVolume.h"
#include "Skybox.h"

#include "vector"

namespace Engine
{

    // Renderer class, subsystem of Engine (Motor) class
    // wraps general OpenGL settings, options and objects
    // prepares and performs rendering operations
    class Renderer
    {
    public:
        Renderer();

        void Render(Engine::Camera& camera, Engine::Actor& actor);
        void Render(Engine::Camera& camera, std::vector<Engine::Actor*> actors);
        void Render(Engine::Camera& camera, Engine::BoundingBox& box);
        void Render(Engine::Camera& camera, Engine::Skybox& skybox);
        void Render(Engine::Camera& camera, std::vector<Engine::Bone*> bones);

        template<typename T>
        void Render(Engine::Camera& camera, Engine::OcTree<T>& tree);
        template<typename T>
        void RecursiveRender(Engine::Camera& camera, Engine::BoundingNode<T>& node);

        std::vector<Engine::Actor*> FrustumCull(Engine::Camera& camera, std::vector<Engine::Actor*> actors);

        void SetColorDepth(int colorDepth);
        int GetColorDepth();

    private:
        int colorDepth;
    };



    // bounding volumes are templated types, we have to define their draw methods here

    template<typename T>
    void Engine::Renderer::RecursiveRender(Engine::Camera& camera, Engine::BoundingNode<T>& node)
    {
        // draw ourselves
        this->Render(camera, node);

        // if we're not a leaf, pass the draw call to the kids
        if (!node.isLeaf)
            for (short i = 0; i < 8; i++)
                this->RecursiveRender(camera , *node.children[i]);
    }

    template<typename T>
    void Engine::Renderer::Render(Engine::Camera& camera, Engine::OcTree<T>& tree)
    {
        RecursiveDraw(&tree.child);
    }

}
#endif //  RENDERER_H
