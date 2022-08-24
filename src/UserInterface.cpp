#include "UserInterface.h"

Engine::Mesh Engine::UserInterface::sharedQuad;

std::vector<Engine::UserInterface::UIElement*> Engine::UserInterface::visibleElements = {};

unsigned int Engine::UserInterface::width = 0;
unsigned int Engine::UserInterface::height = 0;

Engine::Mesh& Engine::UserInterface::GetSharedQuad() const
{
	return sharedQuad;
}

void Engine::UserInterface::Setup(Engine::Window& window)
{
	constexpr const float quadSize = 1.0f;
	constexpr const float quadOffsets = quadSize / 2.0f;

	Engine::UserInterface::width = window.GetDimensions().x;
	Engine::UserInterface::height = window.GetDimensions().y;

	const float ratio = window.GetAspectRatio();

	// construct a fullscreen quad, [quadSize] unit in size, centered around 0.0f
	std::vector<Engine::Vertex> vertices{

		// bottom left
		{ glm::vec3(-quadOffsets , -quadOffsets * ratio, 0.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f, 0.0f)},
		// top left
		{ glm::vec3(-quadOffsets , quadOffsets * ratio, 0.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f, 1.0f) },
		// top right
		{ glm::vec3(quadOffsets  , quadOffsets * ratio, 0.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(1.0f, 1.0f) },
		// bottom right
		{ glm::vec3(quadOffsets  , -quadOffsets * ratio, 0.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(1.0f, 0.0f) } };

	// counter clockwise
	std::vector<unsigned int> indices{ 0, 2, 1,
									   0, 3, 2 };

	Engine::UserInterface::sharedQuad = { std::move(vertices), std::move(indices) };

	Engine::UserInterface::sharedQuad.Setup();

	// we're using complex instances to pack as much UI related data into mat4 as possible
	Engine::UserInterface::sharedQuad.SetInstanceable(true, InstanceType::COMPLEX_INSTANCE);
}

unsigned int Engine::UserInterface::GetWidth()
{
	return Engine::UserInterface::width;
}

unsigned int Engine::UserInterface::GetHeight()
{
	return Engine::UserInterface::height;
}

std::vector<Engine::UserInterface::UIElement*>& Engine::UserInterface::GetVisibleElements()
{
	return Engine::UserInterface::visibleElements;
}

Engine::UserInterface::Panel::Panel(glm::vec2 position, float width, float height)
	: UIElement(position)
{
	this->SetWidth(width);
	this->SetHeight(height);
	this->SetColor(glm::vec4{ 1.0f });

	this->SetBorderColor(glm::vec4{ 1.0f });
	this->SetBorderThickness(0.0f);

	this->SetShader(Engine::ShaderProgram{});

	this->SetVisiblity(true);
}

Engine::UserInterface::Panel::Panel(float x, float y, float width, float height)
	: UIElement(x, y)
{
	this->SetWidth(width);
	this->SetHeight(height);
	this->SetColor(glm::vec4{ 1.0f });

	this->SetBorderColor(glm::vec4{ 1.0f });
	this->SetBorderThickness(0.0f);

	this->SetShader(Engine::ShaderProgram{});

	this->SetVisiblity(true);
}

glm::vec2 Engine::UserInterface::UIElement::GetPosition() const
{
	return this->position;
}

bool Engine::UserInterface::UIElement::GetVisiblity() const
{
	return this->visibility;
}

void Engine::UserInterface::UIElement::SetVisiblity(bool visibility)
{
	// if we're setting the same value, exit early
	if (this->visibility == visibility)
		return;

	this->visibility = visibility;


	// add ourselves to UserInterfaces stack of visible elements
	if (visibility)
		Engine::UserInterface::visibleElements.push_back(this);
	// if we turn invisible, remove us from the stack
	else
	{
		unsigned int stackPointer = this->drawStackPointer;

		for (auto& object : Engine::UserInterface::visibleElements)
		{
			if (object->drawStackPointer > stackPointer) {
				object->drawStackPointer--;
				//object->UpdateDrawStack();
			}

		}

		Engine::UserInterface::visibleElements.erase(
			std::find(Engine::UserInterface::visibleElements.begin(),
				Engine::UserInterface::visibleElements.end(), this));
	}

	this->UpdateDrawStack();

	// do the same for all our children, if we have any
		for (auto& child : this->children)
			SetVisiblity(visibility);
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

float Engine::UserInterface::Panel::GetWidth() const
{
	return width;
}

void Engine::UserInterface::Panel::SetWidth(float value)
{
	width = value;
	this->UpdateDrawStack();
}

float Engine::UserInterface::Panel::GetHeight() const
{
	return height;
}

void Engine::UserInterface::Panel::SetHeight(float value)
{
	height = value;
	this->UpdateDrawStack();
}

Engine::SharedShaderProgram& Engine::UserInterface::Panel::GetShader()
{
	return shader;
}

void Engine::UserInterface::Panel::SetShader(const Engine::SharedShaderProgram& value)
{
	shader = value;
}

glm::vec4 Engine::UserInterface::Panel::GetColor() const
{
	return color;
}

void Engine::UserInterface::Panel::SetColor(glm::vec4 value)
{
	color = value;
	this->UpdateDrawStack();
}

glm::vec4 Engine::UserInterface::Panel::GetBorderColor() const
{
	return borderColor;
}

void Engine::UserInterface::Panel::SetBorderColor(glm::vec4 value)
{
	borderColor = value;
	this->UpdateDrawStack();
}

float Engine::UserInterface::Panel::GetBorderThickness() const
{
	return borderThickness;
}

void Engine::UserInterface::Panel::SetBorderThickness(float value)
{
	borderThickness = value;
	this->UpdateDrawStack();
}

Engine::UserInterface::Layer Engine::UserInterface::UIElement::GetLayer() const
{
	return this->layer;
}

void Engine::UserInterface::UIElement::SetLayer(UserInterface::Layer value)
{
	this->layer = value;
	this->UpdateDrawStack();
}

void Engine::UserInterface::Panel::UpdateDrawStack()
{
	// construct an Engine::ComplexInstance from out data
	Engine::ComplexInstance data = glm::mat4{ { this->position.x, this->position.y, this->width, this->height},
							this->color,
							{this->layer, this->borderThickness, Engine::UserInterface::GetWidth(), Engine::UserInterface::GetHeight()},
							this->borderColor };


	// reference for easier navigation
	auto& drawStack = Engine::UserInterface::sharedQuad.GetInstances<Engine::ComplexInstance>();
	// value considered to be unset
	constexpr const unsigned int unset = -1;

	// check if we already exist on the draw stack
	if (this->drawStackPointer != unset)
		// check if we're visible 
		if (this->visibility) {
			// set our data
			drawStack[drawStackPointer] = data;
		}
		// we're invisible, remove our data and reset the pointer
		else {
			drawStack.erase(drawStack.begin() + this->drawStackPointer);
			this->drawStackPointer = unset;
		}
	else
		// we don't exist on the draw stack yet, but we are visible, and thus should
		if (this->visibility) {
			// add our data to the draw stack and set the pointer to it
			drawStack.push_back(data);
			this->drawStackPointer = drawStack.size() - 1u;
		}
		else
			// no changes are necessary, early return
			return;
	// updating the draw stack is done, upload the new version to the GPU
	glNamedBufferData(Engine::UserInterface::sharedQuad.GetInstancedVertexBufferObject(), sizeof(drawStack[0]) * drawStack.size(), &drawStack[0], GL_DYNAMIC_DRAW);
}

Engine::UserInterface::UIElement::UIElement() : layer(Engine::UserInterface::layers[6])
{
	this->position = glm::ivec2(0);
	this->drawStackPointer = -1;
	this->visibility = false;
	this->parent = nullptr;
}

Engine::UserInterface::UIElement::UIElement(float x, float y) : layer(Engine::UserInterface::layers[6])
{
	this->position = glm::vec2(x, y);
	this->drawStackPointer = -1;
	this->visibility = false;
	this->parent = nullptr;
}

Engine::UserInterface::UIElement::UIElement(glm::vec2 position) : layer(Engine::UserInterface::layers[6])
{
	this->position = position;
	this->drawStackPointer = -1;
	this->visibility = false;
	this->parent = nullptr;
}

void Engine::UserInterface::UIElement::MoveRelative(float x, float y)
{
	// move ourselves
	this->position = { this->position.x + x, this->position.y + y};
	this->UpdateDrawStack();

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

void Engine::UserInterface::UIElement::MoveAbsolute(float x, float y)
{
	// find how much we have to move to get to (x,y) and reuse this->MoveRelative()
	this->MoveRelative(x - this->position.x, y - this->position.y);
}
