#pragma once

#include "Shared.h"
#include "Shader.h"
#include "Mesh.h"

#include "glm/glm.hpp"

#include "vector"
#include "functional"

namespace Engine
{

	// User Interface coordinator
	class UserInterface
	{
	public:
		UserInterface();

		// ==========================================================================
		// basic UI element to be inherited from
		class UIElement
		{
		public:
			UIElement();
			UIElement(int x, int y);
			UIElement(glm::ivec2 position);

			void MoveRelative(int x, int y);
			void MoveAbsolute(int x, int y);

			glm::ivec2 GetPosition() const;
			bool GetVisiblity() const;
			void SetVisiblity(bool);

			void SetParent(UIElement*);
			UIElement* GetParent();

			void AddChild(UIElement*);
			std::vector<UIElement*>& GetChildren();

			virtual void Draw() = 0;

		protected:
			glm::ivec2 position;
			bool visibility;

			UIElement* parent;
			std::vector<UIElement*> children;
		};

		// we're collecting visible elements for easy rendering
		static std::vector<Engine::UserInterface::UIElement*>& GetVisibleElements();
		// ==========================================================================

		// rectangle shaped UI element
		class Panel : public UIElement
		{
		public:
			using UIElement::UIElement;

			Panel(glm::ivec2 origin, unsigned int width, unsigned int height);
			Panel(unsigned int left, unsigned int bottom, unsigned int width, unsigned int height);

			unsigned int GetWidth() const;
			void SetWidth(unsigned int value);

			unsigned int GetHeight() const;
			void SetHeight(unsigned int value);

			Engine::SharedShaderProgram& GetShader();
			void SetShader(const Engine::SharedShaderProgram& value);

			void Draw() override;

		private:
			Engine::SharedShaderProgram shader;

			// bottom left corner
			glm::ivec2 origin;

			unsigned int width;
			unsigned int height;
		};

		// =================================================================

		// mesh consisting of 4 vertices, to be used by UI elements for rendering
		Engine::Mesh quad;

	private:
		static std::vector<UIElement*> visibleElements;
	};

}