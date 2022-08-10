#include "UserInterface.h"

Engine::UserInterface::UserInterface()
{
	// construct the fullscreen quad
	std::vector<Engine::Vertex> vertices {
	{ glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f, 0.0f)},
	{ glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f, 1.0f) },
	{ glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(1.0f, 1.0f) },
	{ glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(1.0f, 0.0f) }};
	
	std::vector<unsigned int> indices{ 0, 2, 1, 0, 3, 2 };
	
	this->quad = { vertices, indices };
}


Engine::UserInterface::Panel::Panel(glm::ivec2 position, unsigned int width, unsigned int height)
	: UIElement(position)
{
	this->SetWidth(width);
	this->SetHeight(height);

	this->SetShader(Engine::ShaderProgram{});
}

Engine::UserInterface::Panel::Panel(unsigned int x, unsigned int y, unsigned int width, unsigned int height)
	: UIElement(x, y)
{
	this->SetWidth(width);
	this->SetHeight(height);

	this->SetShader(Engine::ShaderProgram{});
}

glm::ivec2 Engine::UserInterface::UIElement::GetPosition() const
{
	return this->position;
}

bool Engine::UserInterface::UIElement::GetVisiblity() const
{
	return this->visibility;
}

void Engine::UserInterface::UIElement::SetVisiblity(bool)
{
	this->visibility = visibility;
}

Engine::UserInterface::UIElement* Engine::UserInterface::UIElement::GetParent()
{
	return this->parent;
}

std::vector<Engine::UserInterface::UIElement*> Engine::UserInterface::UIElement::GetChildren()
{

}

unsigned int Engine::UserInterface::Panel::GetWidth() const
{
	return width;
}

void Engine::UserInterface::Panel::SetWidth(unsigned int value)
{
	width = value;
}

unsigned int Engine::UserInterface::Panel::GetHeight() const
{
	return height;
}

void Engine::UserInterface::Panel::SetHeight(unsigned int value)
{
	height = value;
}

Engine::SharedShaderProgram& Engine::UserInterface::Panel::GetShader()
{
	return shader;
}

void Engine::UserInterface::Panel::SetShader(const Engine::SharedShaderProgram& value)
{
	shader = value;
}

Engine::UserInterface::UIElement::UIElement()
{
	this->position = glm::ivec2(0);
	this->visibility = true;
}

Engine::UserInterface::UIElement::UIElement(int x, int y)
{
	this->position = glm::ivec2(x,y);
	this->visibility = true;
}

Engine::UserInterface::UIElement::UIElement(glm::ivec2 position)
{
	this->position = position;
	this->visibility = true;
}

void Engine::UserInterface::UIElement::MoveRelative(int x, int y)
{
	this->position = { this->position.x + x, this->position.y + y};
}

void Engine::UserInterface::UIElement::MoveAbsolute(int x, int y)
{
	this->position = { x, y };
}
