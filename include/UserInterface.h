#pragma once

#include "Shared.h"
#include "Shader.h"
#include "Mesh.h"
#include "Window.h"

#include "glm/glm.hpp"

#include "vector"
#include "functional"

namespace Engine
{

	// User Interface coordinator
	class UserInterface
	{
	public:
		UserInterface() = default;
		
		void Setup(Engine::Window&);

		static Engine::Window::ScreenDimension GetWidth();
		static Engine::Window::ScreenDimension GetHeight();

		// layer type, to be instanced into valid values by UserInterface
		// used by various UI elements as a depth value sent to GPU for rendering
		class Layer
		{
		public:
			Layer(float value) : layer(glm::clamp(value, -1.0f, 1.0f)) {};
			Layer(Layer&&) = delete;
			Layer& operator= (Layer&&) = delete;

			// convenience
			Layer& operator+(const Layer& other) { Layer add(this->layer + other.layer); return add;};
			Layer& operator-(const Layer& other) { Layer sub(this->layer - other.layer); return sub;};
			Layer& operator+(const int i) { Layer add(this->layer + i); return add;};
			Layer& operator-(const int i) { Layer sub(this->layer - i); return sub;};

			Layer(const Layer&) = default;
			Layer& operator= (const Layer&) = default;

			operator float() const { return layer; };
		private:
			float layer;
		};

		// predefined set of 11 layers, all UI elements default to the middle one
		static inline Layer layers[11] = { 0.0f, -0.1f, -0.2f, -0.3f, -0.4f, -0.5f,
										  -0.6f, -0.7f, -0.8f, -0.9f, -1.0f };

		Engine::Mesh& GetSharedQuad() const;

		// ==========================================================================
		// basic UI element to be inherited from
		class UIElement
		{
		public:
			UIElement();
			UIElement(float x, float y);
			UIElement(glm::vec2 position);

			void MoveRelative(float x, float y);
			void MoveAbsolute(float x, float y);

			glm::vec2 GetPosition() const;
			bool GetVisiblity() const;
			void SetVisiblity(bool);

			void SetParent(UIElement*);
			UIElement* GetParent();

			void AddChild(UIElement*);
			std::vector<UIElement*>& GetChildren();

			UserInterface::Layer GetLayer() const;
			void SetLayer(UserInterface::Layer value);

		protected:
			// upload our data to Engine::UserInterface::sharedQuad's instance stack
			virtual void UpdateDrawStack() = 0;

			Layer layer;
			glm::vec2 position;
			
			// pointer into a stack of complex instances
			unsigned int drawStackPointer;
			bool visibility;

			UIElement* parent;
			std::vector<UIElement*> children;
		};

		// we're collecting visible elements for easier rendering
		static std::vector<Engine::UserInterface::UIElement*>& GetVisibleElements();

		// ==========================================================================

		// rectangle shaped UI element
		// panels use Engine::ComplexInstaces to speed up rendering
		// the glm::mat4 of ComplexInstance is packed as follows:
		// 
		// { this->position.x, this->position.y, this->width, this->height }
		// { this->color.r, this->color.g, this->color.b, this->color.a	   }
		// { vec4(this->layer, this->borderThickness vec3(0.0f))		   }
		// { this->borderColor                                             }
		class Panel : public UIElement
		{
		public:
			using UIElement::UIElement;

			Panel(glm::vec2 position, float width, float height);
			Panel(float left, float bottom, float width, float height);

			float GetWidth() const;
			void SetWidth(float value);

			float GetHeight() const;
			void SetHeight(float value);

			Engine::SharedShaderProgram& GetShader();
			void SetShader(const Engine::SharedShaderProgram& value);

			glm::vec4 GetColor() const;
			void SetColor(glm::vec4 value);

			glm::vec4 GetBorderColor() const;
			void SetBorderColor(glm::vec4 value);

			float GetBorderThickness() const;
			void SetBorderThickness(float value);
		private:
			void UpdateDrawStack() override;
			Engine::SharedShaderProgram shader;

			glm::vec4 color;
			glm::vec4 borderColor;
			float borderThickness;
			float width;
			float height;
		};

		// =================================================================

	private:
		// mesh consisting of 4 vertices, to be used by UI elements for instanced rendering
		static Engine::Mesh sharedQuad;

		// stack of visible UI elements
		static std::vector<UIElement*> visibleElements;

		static Engine::Window::WindowDimensions* dimensions;
	};

}