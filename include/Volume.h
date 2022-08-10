#pragma once

#include "Object.h"
#include "Mesh.h"
#include "Texture.h"
#include "Shader.h"

namespace Engine
{

	// a volumetric, renderable object
	// the drawing of these needs to be setup differently compared to regular polygonal geometry
	class Volume : public Engine::Object
	{
		using Engine::Object::Object;

	public:
		Volume();
		~Volume();

		Engine::Mesh& GetMesh();
		Engine::ShaderProgram& GetShader();
		Engine::Texture3D& GetTexture();

		void SetMesh(Engine::Mesh& mesh);
		void SetShader(Engine::ShaderProgram& shader);
		void SetTexture(Engine::Image3D& texture);

	private:
		Engine::SharedStaticMesh mesh;
		Engine::SharedShaderProgram shader;
		Engine::SharedImage3D texture;
	};

}


