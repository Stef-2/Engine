#pragma once

#include "Object.h"
#include "Mesh.h"
#include "Texture3D.h"
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
		Engine::Shader& GetShader();
		Engine::Texture3D& GetTexture();

		void SetMesh(Engine::Mesh& mesh);
		void SetShader(Engine::Shader& shader);
		void SetTexture(Engine::Texture3D& texture);

	private:
		Engine::Mesh mesh;
		Engine::Shader shader;
		Engine::Texture3D texture;
	};

}


