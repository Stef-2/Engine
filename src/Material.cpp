#include "Material.h"

Engine::Material::Material()
{
	diffuse = {};
	roughness = {};
	metallic = {};
	specular = {};
	normal = {};
	alpha = {};

	diffuseMap = {};
	roughnessMap = {};
	metallicMap = {};
	specularMap = {};
	normalMap = {};
	alphaMap = {};
}

Engine::Material::Material(const char* filePath)
{
	diffuse = {};
	roughness = {};
	metallic = {};
	specular = {};
	normal = {};
	alpha = {};

	diffuseMap = {};
	roughnessMap = {};
	metallicMap = {};
	specularMap = {};
	normalMap = {};
	alphaMap = {};
}

void Engine::Material::Activate()
{
	if (this->diffuseMap.GetTextureID())
	{
		glActiveTexture(GL_TEXTURE0 + 0);
		glBindTexture(GL_TEXTURE_2D, this->diffuseMap.GetTextureID());
		glUniform1i(Shader::GetCurrentShader().GetAttributeLocation(Engine::Shader::ShaderAttribute::DIFFUSE_MAP), 0);
	}

	if (this->roughnessMap.GetTextureID())
	{
		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, this->roughnessMap.GetTextureID());
		glUniform1i(Shader::GetCurrentShader().GetAttributeLocation(Engine::Shader::ShaderAttribute::ROUGHNESS_MAP), 1);
	}

	if (this->metallicMap.GetTextureID())
	{
		glActiveTexture(GL_TEXTURE0 + 2);
		glBindTexture(GL_TEXTURE_2D, this->metallicMap.GetTextureID());
		glUniform1i(Shader::GetCurrentShader().GetAttributeLocation(Engine::Shader::ShaderAttribute::METALLIC_MAP), 2);
	}

	if (this->specularMap.GetTextureID())
	{
		glActiveTexture(GL_TEXTURE0 + 3);
		glBindTexture(GL_TEXTURE_2D, this->specularMap.GetTextureID());
		glUniform1i(Shader::GetCurrentShader().GetAttributeLocation(Engine::Shader::ShaderAttribute::SPECULAR_MAP), 3);
	}

	if (this->normalMap.GetTextureID())
	{
		glActiveTexture(GL_TEXTURE0 + 4);
		glBindTexture(GL_TEXTURE_2D, this->normalMap.GetTextureID());
		glUniform1i(Shader::GetCurrentShader().GetAttributeLocation(Engine::Shader::ShaderAttribute::NORMAL_MAP), 4);
	}

	if (this->alphaMap.GetTextureID())
	{
		glActiveTexture(GL_TEXTURE0 + 5);
		glBindTexture(GL_TEXTURE_2D, this->alphaMap.GetTextureID());
		glUniform1i(Shader::GetCurrentShader().GetAttributeLocation(Engine::Shader::ShaderAttribute::ALPHA_MAP), 5);
	}
}

void Engine::Material::Activate(Engine::Shader& shader)
{
	if (this->diffuseMap.GetTextureID())
	{
		glActiveTexture(GL_TEXTURE0 + 0);
		glBindTexture(GL_TEXTURE_2D, this->diffuseMap.GetTextureID());
		glUniform1i(shader.GetAttributeLocation(Engine::Shader::ShaderAttribute::DIFFUSE_MAP), 0);
	}

	if (this->roughnessMap.GetTextureID())
	{
		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, this->roughnessMap.GetTextureID());
		glUniform1i(shader.GetAttributeLocation(Engine::Shader::ShaderAttribute::ROUGHNESS_MAP), 1);
	}

	if (this->metallicMap.GetTextureID())
	{
		glActiveTexture(GL_TEXTURE0 + 2);
		glBindTexture(GL_TEXTURE_2D, this->metallicMap.GetTextureID());
		glUniform1i(shader.GetAttributeLocation(Engine::Shader::ShaderAttribute::METALLIC_MAP), 2);
	}

	if (this->specularMap.GetTextureID())
	{
		glActiveTexture(GL_TEXTURE0 + 3);
		glBindTexture(GL_TEXTURE_2D, this->specularMap.GetTextureID());
		glUniform1i(shader.GetAttributeLocation(Engine::Shader::ShaderAttribute::SPECULAR_MAP), 3);
	}

	if (this->normalMap.GetTextureID())
	{
		glActiveTexture(GL_TEXTURE0 + 4);
		glBindTexture(GL_TEXTURE_2D, this->normalMap.GetTextureID());
		glUniform1i(shader.GetAttributeLocation(Engine::Shader::ShaderAttribute::NORMAL_MAP), 4);
	}

	if (this->alphaMap.GetTextureID())
	{
		glActiveTexture(GL_TEXTURE0 + 5);
		glBindTexture(GL_TEXTURE_2D, this->alphaMap.GetTextureID());
		glUniform1i(shader.GetAttributeLocation(Engine::Shader::ShaderAttribute::ALPHA_MAP), 5);
	}
}

void Engine::Material::SetDiffuseMap(const char* filePath)
{
	this->diffuseMap = Engine::Texture(filePath);
}

void Engine::Material::SetDiffuseMap(std::string filePath)
{
	this->diffuseMap = Engine::Texture(filePath);
}

void Engine::Material::SetDiffuseMap(const Texture& texture)
{
	this->diffuseMap = texture;
}

void Engine::Material::SetRoughnessMap(const char* filePath)
{
	this->roughnessMap = Engine::Texture(filePath);
}

void Engine::Material::SetRoughnessMap(std::string filePath)
{
	this->diffuseMap = Engine::Texture(filePath);
}

void Engine::Material::SetRoughnessMap(const Texture& texture)
{
	this->roughnessMap = texture;
}

void Engine::Material::SetMetallicMap(const char* filePath)
{
	this->metallicMap = Engine::Texture(filePath);
}

void Engine::Material::SetMetallicMap(std::string filePath)
{
	this->diffuseMap = Engine::Texture(filePath);
}

void Engine::Material::SetMetallicMap(const Texture& texture)
{
	this->metallicMap = texture;
}

void Engine::Material::SetNormalMap(const char* filePath)
{
	this->normalMap = Engine::Texture(filePath);
}

void Engine::Material::SetNormalMap(std::string filePath)
{
	this->normalMap = Engine::Texture(filePath);
}

void Engine::Material::SetNormalMap(const Texture& texture)
{
	this->normalMap = texture;
}

Engine::Texture& Engine::Material::GetDiffuseMap()
{
	return this->diffuseMap;
}

Engine::Texture& Engine::Material::GetRougnessMap()
{
	return this->roughnessMap;
}

Engine::Texture& Engine::Material::GetMetallicMap()
{
	return this->metallicMap;
}

Engine::Texture& Engine::Material::GetSpecularMap()
{
	return this->specularMap;
}

Engine::Texture& Engine::Material::GetNormalMap()
{
	return this->normalMap;
}

Engine::Texture& Engine::Material::GetAlphaMap()
{
	return this->alphaMap;
}
