#pragma once

#include "Output.h"

#include "glm/glm.hpp"
#include "glm/gtx/color_space.hpp"

#include "string"

namespace Engine
{
	// color storage class, extending glm::vec4, matching GLSL vec4
	// stores red, green, blue, alpha values by default and a ColorMode indicating RGB or HSV encoding
	class Color : public glm::vec4
	{
	public:
		// reuse glm::vec constructors
		using glm::vec4::vec4;

		// construction types
		struct hex24 {const unsigned int color;};
		struct hex32 {const unsigned int color;};

		// strings taking format [#RRGGBB] or [#RRGGBBAA]
		struct hexString24 {const char color[1 + 6];};
		struct hexString32 {const char color[1 + 8];};

		enum class ColorMode
		{
			RGB,
			HSV,
			HSL = HSV,
			DEFAULT = RGB
		};

		// constructible from glm::vectors
		Color(glm::vec3, ColorMode = ColorMode::DEFAULT);
		Color(glm::vec4, ColorMode = ColorMode::DEFAULT);
		Color(glm::ivec4, ColorMode = ColorMode::DEFAULT);
		Color(glm::ivec3, ColorMode = ColorMode::DEFAULT);

		// constructible from HTML-like hex codes
		// 24 bit variant for RGB
		Color(hex24, ColorMode = ColorMode::DEFAULT);
		// 32 bit variant for RGBA
		Color(hex32, ColorMode = ColorMode::DEFAULT);

		// constructible from HTML-like hashtag strings taking format #RRGGBB or #RRGGBBAA
		Color(hexString24, ColorMode = ColorMode::DEFAULT);
		Color(hexString32, ColorMode = ColorMode::DEFAULT);

		// individual channels
		// rgb mode
		float& Red();
		float& Green();
		float& Blue();

		// hsv mode
		float& Hue();
		float& Saturation();
		float& Value();

		float& Alpha();

		// implicit conversion to glm::vec4
		operator glm::vec4();
		// explicit conversion to hex code
		explicit operator unsigned int();
		// explicit conversion to hex string
		explicit operator std::string();

		Color& Mix(const Color& other, float scale = 0.5f) const;

		Color& GetHSV() const;
		Color& GetRGB() const;

		void ConvertTo(ColorMode);

		// predefined common colors =================================================
		
		// grayscale
		static const Color white;
		static const Color black;
		static const Color gray;
		static const Color transparent;

		// primaries
		static const Color red;
		static const Color green;
		static const Color blue;

		// secondaries
		static const Color yellow;
		static const Color cyan;
		static const Color magenta;

		// tertiaries 
		static const Color redYellow;
		static const Color greenYellow;
		static const Color greenCyan;
		static const Color blueCyan;
		static const Color redMagenta;
		static const Color blueMagenta;

		// regular / common names for tertiaries ^
		static const Color& orange;
		static const Color& chartreuse;
		static const Color& springGreen;
		static const Color& azure;
		static const Color& violet;
		static const Color& rose;

		// ==========================================================================

	private:

		ColorMode mode;

		inline static const constexpr int OxFF = 0xFF;
	};


}

