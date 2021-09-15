#ifndef RENDERER_H
#define RENDERER_H

#include "Camera.h"
#include "Actor.h"
#include "BoundingVolume.h"
#include "Skybox.h"
#include "Shader.h"
#include "Light.h"

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

        void Render(const Engine::Camera& camera, Engine::Actor& actor);
        void Render(const Engine::Camera& camera, const std::vector<Engine::Actor*>& actors);
        void Render(const Engine::Camera& camera, const Engine::BoundingBox& box);
        void Render(const Engine::Camera& camera, Engine::Skybox& skybox);
        void Render(const Engine::Camera& camera, Engine::Skeleton& skeleton);

        void RenderAnimated(const Engine::Camera& camera, Engine::Actor& actor);
        void RenderAnimated(const Engine::Camera& camera, const std::vector<Engine::Actor*>& actors);

        template<typename T>
        void Render(const Engine::Camera& camera, const Engine::OcTree<T>& tree);
        template<typename T>
        void RecursiveRender(const Engine::Camera& camera, const Engine::BoundingNode<T>& node);

        std::vector<Engine::Actor*> FrustumCull(const Engine::Camera& camera, const std::vector<Engine::Actor*> actors);

        void SetColorDepth(const int colorDepth);
        int GetColorDepth();

    private:
        // wireframe utility shader, meant to be used for bounding volume rendering -
        // no real point in defining it outside, or attaching it to individual bounding box objects - 
        // they are meant to be lightweight by design, and bounding box rendering is intended to be mostly used for debugging
        Shader wireframeShader;
        int colorDepth;
    };



    // bounding volumes are templated types, we have to define their methods here

    template<typename T>
    void Engine::Renderer::RecursiveRender(const Engine::Camera& camera, const Engine::BoundingNode<T>& node)
    {
        // draw ourselves
        this->Render(camera, node);

        // if we're not a leaf, pass the draw call to the kids
        if (!node.isLeaf)
            for (short i = 0; i < 8; i++)
                this->RecursiveRender(camera , *node.children[i]);
    }

    template<typename T>
    void Engine::Renderer::Render(const Engine::Camera& camera, const Engine::OcTree<T>& tree)
    {
        RecursiveDraw(&tree.child);
    }

}
#endif //  RENDERER_H
