#include "UserInterface.h"

std::vector<Engine::UserInterface::UIElement*> Engine::UserInterface::visibleElements = {};

Engine::UserInterface::UserInterface()
{
	// construct the fullscreen quad
	std::vector<Engine::Vertex> vertices {
	{ glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f, 0.0f)},
	{ glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f, 1.0f) },
	{ glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(1.0f, 1.0f) },
	{ glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(1.0f, 0.0f) }};
	
	std::vector<unsigned int> indices{ 0, 2, 1, 0, 3, 2 };
	
	this->quad = { std::move(vertices), std::move(indices) };
}


std::vector<Engine::UserInterface::UIElement*>& Engine::UserInterface::GetVisibleElements()
{
	return Engine::UserInterface::visibleElements;
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

void Engine::UserInterface::UIElement::SetVisiblity(bool visibility)
{
	this->visibility = visibility;

	if (visibility)
		Engine::UserInterface::visibleElements.push_back(this);
	else
		Engine::UserInterface::visibleElements.erase(
			std::find(Engine::UserInterface::visibleElements.begin(),
				Engine::UserInterface::visibleElements.end(), this));
}

void Engine::UserInterface::UIElement::SetParent(UIElement* obj)
{
	this->parent = obj;
}

Engine::UserInterface::UIElement* Engine::UserInterface::UIElement::GetParent()
{
	return this->parent;
}

void Engine::UserInterface::UIElement::AddChild(UIElement* obj)
{
	this->children.push_back(obj);
}

std::vector<Engine::UserInterface::UIElement*>& Engine::UserInterface::UIElement::GetChildren()
{
	return this->children;
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

void Engine::UserInterface::Panel::Draw()
{

}

Engine::UserInterface::UIElement::UIElement()
{
	this->position = glm::ivec2(0);
	this->SetVisiblity(true);
}

Engine::UserInterface::UIElement::UIElement(int x, int y)
{
	this->position = glm::ivec2(x,y);
	this->SetVisiblity(true);
}

Engine::UserInterface::UIElement::UIElement(glm::ivec2 position)
{
	this->position = position;
	this->SetVisiblity(true);
}

void Engine::UserInterface::UIElement::MoveRelative(int x, int y)
{
	// move ourselves
	this->position = { this->position.x + x, this->position.y + y};

	// recursive lambda that will traverse the child tree and move them as well
	std::function<void(UIElement*)> TraverseDown = [&TraverseDown, &x, &y](UIElement* parent)
	{
		// if we have any kids, move them as well
		for (auto& kid : parent->children) {
			kid->MoveRelative(x, y);
			TraverseDown(kid);
		}
	};

	// move the kids
	TraverseDown(this);
}

void Engine::UserInterface::UIElement::MoveAbsolute(int x, int y)
{
	// find how much we have to move to get to (x,y) and reuse MoveRelative()
	this->MoveRelative(x - this->position.x, y - this->position.y);
}
