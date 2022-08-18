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
	if (this->diffuseMap && this->diffuseMap->GetTextureID() && ShaderProgram::GetCurrentShaderProgram().GetAttributeLocation(Engine::ShaderProgram::ShaderAttribute::DIFFUSE_MAP) >= -1)
	{
		glActiveTexture(GL_TEXTURE0 + 0);
		glBindTexture(GL_TEXTURE_2D, this->diffuseMap->GetTextureID());
		glUniform1i(ShaderProgram::GetCurrentShaderProgram().GetAttributeLocation(Engine::ShaderProgram::ShaderAttribute::DIFFUSE_MAP), 0);
	}

	if (this->diffuseMap && this->diffuseMap->GetTextureID() && ShaderProgram::GetCurrentShaderProgram().GetAttributeLocation(Engine::ShaderProgram::ShaderAttribute::ROUGHNESS_MAP) >= -1)
	{
		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, this->roughnessMap->GetTextureID());
		glUniform1i(ShaderProgram::GetCurrentShaderProgram().GetAttributeLocation(Engine::ShaderProgram::ShaderAttribute::ROUGHNESS_MAP), 1);
	}

	if (this->diffuseMap && this->diffuseMap->GetTextureID() && ShaderProgram::GetCurrentShaderProgram().GetAttributeLocation(Engine::ShaderProgram::ShaderAttribute::METALLIC_MAP) >= -1)
	{
		glActiveTexture(GL_TEXTURE0 + 2);
		glBindTexture(GL_TEXTURE_2D, this->metallicMap->GetTextureID());
		glUniform1i(ShaderProgram::GetCurrentShaderProgram().GetAttributeLocation(Engine::ShaderProgram::ShaderAttribute::METALLIC_MAP), 2);
	}

	if (this->diffuseMap && this->diffuseMap->GetTextureID() && ShaderProgram::GetCurrentShaderProgram().GetAttributeLocation(Engine::ShaderProgram::ShaderAttribute::SPECULAR_MAP) >= -1)
	{
		glActiveTexture(GL_TEXTURE0 + 3);
		glBindTexture(GL_TEXTURE_2D, this->specularMap->GetTextureID());
		glUniform1i(ShaderProgram::GetCurrentShaderProgram().GetAttributeLocation(Engine::ShaderProgram::ShaderAttribute::SPECULAR_MAP), 3);
	}

	if (this->diffuseMap && this->diffuseMap->GetTextureID() && ShaderProgram::GetCurrentShaderProgram().GetAttributeLocation(Engine::ShaderProgram::ShaderAttribute::NORMAL_MAP) >= -1)
	{
		glActiveTexture(GL_TEXTURE0 + 4);
		glBindTexture(GL_TEXTURE_2D, this->normalMap->GetTextureID());
		glUniform1i(ShaderProgram::GetCurrentShaderProgram().GetAttributeLocation(Engine::ShaderProgram::ShaderAttribute::NORMAL_MAP), 4);
	}

	if (this->diffuseMap && this->diffuseMap->GetTextureID() && ShaderProgram::GetCurrentShaderProgram().GetAttributeLocation(Engine::ShaderProgram::ShaderAttribute::ALPHA_MAP) >= -1)
	{
		glActiveTexture(GL_TEXTURE0 + 5);
		glBindTexture(GL_TEXTURE_2D, this->alphaMap->GetTextureID());
		glUniform1i(ShaderProgram::GetCurrentShaderProgram().GetAttributeLocation(Engine::ShaderProgram::ShaderAttribute::ALPHA_MAP), 5);
	}
}

void Engine::Material::Activate(Engine::ShaderProgram& shader)
{
	if (this->diffuseMap && this->diffuseMap->GetTextureID() && shader.GetAttributeLocation(Engine::ShaderProgram::ShaderAttribute::DIFFUSE_MAP) >= -1)
	{
		glActiveTexture(GL_TEXTURE0 + 0);
		glBindTexture(GL_TEXTURE_2D, this->diffuseMap->GetTextureID());
		glUniform1i(shader.GetAttributeLocation(Engine::ShaderProgram::ShaderAttribute::DIFFUSE_MAP), 0);
	}

	if (this->roughnessMap && this->roughnessMap->GetTextureID() && shader.GetAttributeLocation(Engine::ShaderProgram::ShaderAttribute::ROUGHNESS_MAP) >= -1)
	{
		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, this->roughnessMap->GetTextureID());
		glUniform1i(shader.GetAttributeLocation(Engine::ShaderProgram::ShaderAttribute::ROUGHNESS_MAP), 1);
	}

	if (this->metallicMap && this->metallicMap->GetTextureID() && shader.GetAttributeLocation(Engine::ShaderProgram::ShaderAttribute::METALLIC_MAP) >= -1)
	{
		glActiveTexture(GL_TEXTURE0 + 2);
		glBindTexture(GL_TEXTURE_2D, this->metallicMap->GetTextureID());
		glUniform1i(shader.GetAttributeLocation(Engine::ShaderProgram::ShaderAttribute::METALLIC_MAP), 2);
	}

	if (this->specularMap && this->specularMap->GetTextureID() && shader.GetAttributeLocation(Engine::ShaderProgram::ShaderAttribute::SPECULAR_MAP) >= -1)
	{
		glActiveTexture(GL_TEXTURE0 + 3);
		glBindTexture(GL_TEXTURE_2D, this->specularMap->GetTextureID());
		glUniform1i(shader.GetAttributeLocation(Engine::ShaderProgram::ShaderAttribute::SPECULAR_MAP), 3);
	}

	if (this->normalMap &&this->normalMap->GetTextureID() && shader.GetAttributeLocation(Engine::ShaderProgram::ShaderAttribute::NORMAL_MAP) >= -1)
	{
		glActiveTexture(GL_TEXTURE0 + 4);
		glBindTexture(GL_TEXTURE_2D, this->normalMap->GetTextureID());
		glUniform1i(shader.GetAttributeLocation(Engine::ShaderProgram::ShaderAttribute::NORMAL_MAP), 4);
	}

	if (this->alphaMap && this->alphaMap->GetTextureID() && shader.GetAttributeLocation(Engine::ShaderProgram::ShaderAttribute::ALPHA_MAP) >= -1)
	{
		glActiveTexture(GL_TEXTURE0 + 5);
		glBindTexture(GL_TEXTURE_2D, this->alphaMap->GetTextureID());
		glUniform1i(shader.GetAttributeLocation(Engine::ShaderProgram::ShaderAttribute::ALPHA_MAP), 5);
	}
}

void Engine::Material::SetDiffuseMap(const char* filePath)
{
	this->diffuseMap = std::make_shared<Engine::Texture2D>(Engine::Texture2D(filePath));
}

void Engine::Material::SetDiffuseMap(std::string filePath)
{
	this->diffuseMap = std::make_shared<Engine::Texture2D>(Engine::Texture2D(filePath));
}

void Engine::Material::SetDiffuseMap(const Texture2D& texture)
{
	this->diffuseMap = std::make_shared<Engine::Texture2D>(texture);
}

void Engine::Material::SetRoughnessMap(const char* filePath)
{
	this->roughnessMap = std::make_shared<Engine::Texture2D>(Engine::Texture2D(filePath));
}

void Engine::Material::SetRoughnessMap(std::string filePath)
{
	this->roughnessMap = std::make_shared<Engine::Texture2D>(Engine::Texture2D(filePath));
}

void Engine::Material::SetRoughnessMap(const Texture2D& texture)
{
	this->roughnessMap = std::make_shared<Engine::Texture2D>(texture);
}

void Engine::Material::SetMetallicMap(const char* filePath)
{
	this->metallicMap = std::make_shared<Engine::Texture2D>(Engine::Texture2D(filePath));
}

void Engine::Material::SetMetallicMap(std::string filePath)
{
	this->metallicMap = std::make_shared<Engine::Texture2D>(Engine::Texture2D(filePath));
}

void Engine::Material::SetMetallicMap(const Texture2D& texture)
{
	this->metallicMap = std::make_shared<Engine::Texture2D>(texture);
}

void Engine::Material::SetNormalMap(const char* filePath)
{
	this->normalMap = std::make_shared<Engine::Texture2D>(Engine::Texture2D(filePath));
}

void Engine::Material::SetNormalMap(std::string filePath)
{
	this->normalMap = std::make_shared<Engine::Texture2D>(Engine::Texture2D(filePath));
}

void Engine::Material::SetNormalMap(const Texture2D& texture)
{
	this->normalMap = std::make_shared<Engine::Texture2D>(texture);
}

Engine::Texture2D& Engine::Material::GetDiffuseMap()
{
	return *this->diffuseMap;
}

Engine::Texture2D& Engine::Material::GetRougnessMap()
{
	return *this->roughnessMap;
}

Engine::Texture2D& Engine::Material::GetMetallicMap()
{
	return *this->metallicMap;
}

Engine::Texture2D& Engine::Material::GetSpecularMap()
{
	return *this->specularMap;
}

Engine::Texture2D& Engine::Material::GetNormalMap()
{
	return *this->normalMap;
}

Engine::Texture2D& Engine::Material::GetAlphaMap()
{
	return *this->alphaMap;
}
