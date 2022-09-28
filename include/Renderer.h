#ifndef RENDERER_H
#define RENDERER_H

#include "Camera.h"
#include "Actor.h"
#include "BoundingVolume.h"
#include "Skybox.h"
#include "Shader.h"
#include "Light.h"
#include "Terrain.h"
#include "FrameBuffer.h"
#include "Volume.h"
#include "UserInterface.h"

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

		enum class QueryableProperty
		{
			MAX_TOTAL_TEXTURE_UNITS,
			MAX_FRAGMENT_TEXTURE_UNITS,
			MAX_1D_TEXTURE_SIZE,
			MAX_2D_TEXTURE_SIZE,
			MAX_3D_TEXTURE_SIZE,
			MAX_CUBEMAP_TEXTURE_SIZE,
			MAX_ARRAY_TEXTURE_LAYERS,
			MAX_FRAMEBUFFER_WIDTH,
			MAX_FRAMEBUFFER_HEIGHT,
			MAX_RENDERBUFFER_SIZE,
			MAX_TEXTUREBUFFER_SIZE,
			MAX_FRAGMENT_INPUTS,
			MAX_VERTEX_ATTRIBUTES,
			MAX_VERTEX_OUTPUTS,
		};

		void Render(const Engine::Camera&, Engine::Actor& actor);
		void Render(const Engine::Camera&, const std::vector<Engine::Actor*>& actors);
		void Render(const Engine::Camera&, const Engine::BoundingBox& box);
		void Render(const Engine::Camera&, Engine::Skybox& skybox);
		void Render(const Engine::Camera&, Engine::Skeleton& skeleton);
		void Render(const Engine::Camera&, Engine::PointLight& light);
		void Render(const Engine::Camera&, Engine::Terrain& terrain);
		void Render(const Engine::Camera&, Engine::Volume& volume);
		
		void Render(Engine::UserInterface& UI);

		// shadow mapping
		void Render(const Engine::DirectionalLight& light, Engine::FrameBuffer& shadowBuffer, Engine::Actor& actor);
		void Render(const Engine::DirectionalLight& light, const Engine::FrameBuffer& shadowBuffer, const std::vector<Engine::Actor*>& actors) {};

		template<typename T>
		void Render(const Engine::Camera& camera, const Engine::OcTree<T>& tree);
		template<typename T>
		void RecursiveRender(const Engine::Camera& camera, const Engine::BoundingNode<T>& node);

		std::vector<Engine::Actor*> FrustumCull(const Engine::Camera& camera, const std::vector<Engine::Actor*> actors);

		void SetColorDepth(const int colorDepth);

		unsigned short GetColorDepth();
		unsigned int QueryProperty(QueryableProperty property);

		// check if the system supports a particular OpenGL extension, provided as a string
		bool IsExtensionSupported(std::string& extensionName);

	private:
		unsigned short colorDepth;

		// OpenGL implementation limits on this system
		int maxTotalTextureUnits;
		int maxFragmentTextureUnits;
		int max1DTextureSize;
		int max2DTextureSize;
		int max3DTextureSize;
		int maxCubMapTextureSize;
		int maxArrayTextureLayers;
		int maxFrameBufferWidth;
		int maxFrameBufferHeight;
		int maxRenderBufferSize;
		int maxTextureBuferSize;
		int maxFragmentInputs;
		int maxVertexAttributes;
		int maxVertexOutputs;

		// utility shaders

		// wireframe utility shader, meant to be used for bounding volume rendering -
		// no real point in defining it outside, or attaching it to individual bounding box objects - 
		// they are meant to be lightweight by design, and bounding box rendering is intended to be mostly used for debugging
		ShaderProgram wireFrameShader;

		// shadow mapping shader, used for dynamic shadows
		ShaderProgram shadowMapShader;
	};

	// ==========================================================================
	// bounding volumes are templated types, we have to define their methods here
	// ==========================================================================

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
