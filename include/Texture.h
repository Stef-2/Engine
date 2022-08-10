#ifndef TEXTURE_H
#define TEXTURE_H

#ifndef __glad_h_
#include "glad/glad.h"
#endif

#ifndef STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_IMPLEMENTATION
#endif

#include "Shared.h"
#include "Shader.h"

#include "glfw3.h"
#include "fstream"
#include "iostream"

namespace Engine
{
	// texture loading and handling class
	class Texture2D
	{
	public:
		Texture2D();

		// regular textures
		Texture2D(std::string filePath);
		Texture2D(std::string filePaths[6]);
 
		Texture2D(Texture2D&&) noexcept = default;
		Texture2D& operator=(Texture2D&&) noexcept = default;

		Texture2D(const Texture2D&) = default;
		Texture2D& operator=(const Texture2D&) = default;

		~Texture2D();

		// loads the texture from a file
		// and binds it into an OpenGL texture object
		void Setup(std::string filePath);

		// loads 6 texture files for skyboxes (Cubemaps)
		// order is left, right, top, bottom, front, back
		void Setup(std::string filePaths[6]);

		// this is the handle to the texture object
		// pass it to OpenGL before any rendering using this texture
		unsigned int GetTextureID();
		void SetTextureID(unsigned int id);

		// activate the texture
		virtual void Activate();
			
		// raw texture data extracted from the file
		unsigned char* GetData();
		void SetData(unsigned char* data);

		unsigned int GetWidth();
		unsigned int GetHeight();
		unsigned int GetNumChannels();

		void SetWidth(unsigned int width);
		void SetHeight(unsigned int height);
		void SetNumChannels(unsigned int numChannels);

	protected:
		virtual void GenerateTexture();

		unsigned int width;
		unsigned int height;
		unsigned int numChannels;
		unsigned char* data;
		unsigned int textureID;
	};

	// ===========================================================================

	// 2D image class for writing to and from shaders, extends Texture2D
	class Image2D : public Engine::Texture2D
	{
	public:
		// we don't want to be constructible from a texture like Texture2D
		Image2D(std::string) = delete;

		Image2D(unsigned int width, unsigned int height);
		Image2D(unsigned int width, unsigned int height, unsigned short numChannels);

		void Activate() override;
	private:
		void GenerateTexture() override;
	};

	// ===========================================================================

	// extends a 2D texture with one more dimension
	class Texture3D : public Texture2D
	{
	public:
		Texture3D() = default;
		Texture3D(std::string filePath);

		Texture3D(Texture3D&&) noexcept = default;
		Texture3D& operator=(Texture3D&&) noexcept = default;

		Texture3D(const Texture3D&) = default;
		Texture3D& operator=(const Texture3D&) = default;

		// activate the texture
		void Activate() override;

		unsigned int GetDepth();

		void SetDepth(unsigned int depth);

	protected:
		void GenerateTexture() override;

		unsigned int depth;
	};

	// ===========================================================================
	
	// 3D image class for writing to and from shaders, extends Texture3D
	class Image3D : public Engine::Texture3D
	{
	public:
		// we don't want to be constructible from a texture like Texture2D
		Image3D(std::string) = delete;

		Image3D(unsigned int width, unsigned int height, unsigned int depth);
		Image3D(unsigned int width, unsigned int height, unsigned int depth, unsigned short numChannels);

		void Compute(Engine::ShaderProgram&);
		void Compute(Engine::ComputeShader&);

		void Activate() override;
	private:
		void GenerateTexture() override;
	};

	// ===========================================================================

	// wrapper for texture related enums
	class Texture
	{
		friend Engine::Texture2D;
		friend Engine::Texture3D;

		typedef enum class TextureType : GLenum
		{
			TEXTURE1D = GL_TEXTURE_1D,
			TEXTURE2D = GL_TEXTURE_2D,
			TEXTURE3D = GL_TEXTURE_3D,
			CUBEMAP = GL_TEXTURE_CUBE_MAP
		};
	};

	// typdef for shared textures
	using SharedTexture2D = Engine::Shared<Engine::Texture2D>;
	using SharedTexture3D = Engine::Shared<Engine::Texture3D>;

	using SharedImage2D = Engine::Shared<Engine::Image2D>;
	using SharedImage3D = Engine::Shared<Engine::Image3D>;
}
#endif //  TEXTURE_H
