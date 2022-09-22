#include "Color.h"

// grayscales
const Engine::Color Engine::Color::white {1.0f, 1.0f, 1.0f, 1.0f};
const Engine::Color Engine::Color::black {0.0f, 0.0f, 0.0f, 1.0f};
const Engine::Color Engine::Color::gray {0.5f, 0.5f, 0.5f, 1.0f};
const Engine::Color Engine::Color::transparent {0.0f, 0.0f, 0.0f, 0.0f};

// primaries
const Engine::Color Engine::Color::red {1.0f, 0.0f, 0.0f, 1.0f};
const Engine::Color Engine::Color::green {0.0f, 1.0f, 0.0f, 1.0f};
const Engine::Color Engine::Color::blue {0.0f, 0.0f, 1.0f, 1.0f};

// secondaries
const Engine::Color Engine::Color::yellow {1.0f, 1.0f, 0.0f, 1.0f};
const Engine::Color Engine::Color::cyan {0.0f, 1.0f, 1.0f, 1.0f};
const Engine::Color Engine::Color::magenta {1.0f, 0.0f, 1.0f, 1.0f};

const Engine::Color Engine::Color::redYellow {red.Mix(yellow)};
const Engine::Color Engine::Color::greenYellow {green.Mix(yellow)};
const Engine::Color Engine::Color::greenCyan {green.Mix(cyan)};
const Engine::Color Engine::Color::blueCyan {blue.Mix(cyan)};
const Engine::Color Engine::Color::redMagenta {red.Mix(magenta)};
const Engine::Color Engine::Color::blueMagenta{blue.Mix(magenta)};

// regular names for tertiaries
const Engine::Color& Engine::Color::orange {redYellow};
const Engine::Color& Engine::Color::chartreuse {greenYellow};
const Engine::Color& Engine::Color::springGreen {greenCyan};
const Engine::Color& Engine::Color::azure {blueCyan};
const Engine::Color& Engine::Color::rose {redMagenta};
const Engine::Color& Engine::Color::violet {blueMagenta};

// ============================================================================

Engine::Color::Color(glm::vec4 color, ColorMode mode)
	: vec(color), mode(mode)
{

}

Engine::Color::Color(glm::ivec3 color, ColorMode mode)
	: vec((color / OxFF), 1.0f), mode(mode)
{

}

Engine::Color::Color(glm::ivec4 color, ColorMode mode)
	:vec(color / OxFF), mode(mode)
{
	
}

Engine::Color::Color(glm::vec3 color, ColorMode mode)
	: vec(color, 1.0f), mode(mode)
{

}

float& Engine::Color::Red()
{
	if (this->mode == ColorMode::RGB)
		return this->r;

		return this->GetRGB().r;
}

float& Engine::Color::Green()
{
	if (this->mode == ColorMode::RGB)
		return this->g;

	return this->GetRGB().g;
}

float& Engine::Color::Blue()
{
	if (this->mode == ColorMode::RGB)
		return this->b;

	return this->GetRGB().b;
}

float& Engine::Color::Hue()
{
	if (this->mode == ColorMode::HSV)
		return this->r;

	return this->GetHSV().r;
}

float& Engine::Color::Saturation()
{
	if (this->mode == ColorMode::HSV)
		return this->g;

	return this->GetHSV().g;
}

float& Engine::Color::Value()
{
	if (this->mode == ColorMode::HSV)
		return this->b;

	return this->GetHSV().b;
}

float& Engine::Color::Alpha()
{
	return this->GetRGB().a;
}

Engine::Color::Color(hex24 hex, ColorMode mode)
	: vec(), mode(mode)
{
	this->r = ((hex.color >> 24) & OxFF) / OxFF;
	this->g = ((hex.color >> 16) & OxFF) / OxFF;
	this->b = ((hex.color >> 8) & OxFF) / OxFF;
	this->a = 1.0f;
}

Engine::Color::Color(hex32 hex, ColorMode mode)
	: vec(), mode(mode)
{
	this->r = ((hex.color >> 24) & OxFF) / OxFF;
	this->g = ((hex.color >> 16) & OxFF) / OxFF;
	this->b = ((hex.color >> 8) & OxFF) / OxFF;
	this->a = ((hex.color >> 0) & OxFF) / OxFF;
}

Engine::Color::Color(hexString24 string, ColorMode mode)
	:vec(), mode(mode)
{
	// check if the string begins with a hashtag, god knows why
	if (string.color[0] != '#') {
		Output::Error() << "incorrect color string format, correct format is [#RRGGBB]";
		throw std::invalid_argument::invalid_argument
			("incorrect color string format, correct format is [#RRGGBB]");
	}
	
	// convert everything past the hashtag into a number and construct us from that
	unsigned int hex = std::atoi(&string.color[1]);

	*this = Color{hex24{hex}};
}

Engine::Color::Color(hexString32 string, ColorMode mode)
	:vec(), mode(mode)
{
	// check if the string begins with a hashtag, god knows why
	if (string.color[0] != '#') {
		Output::Error() << "incorrect color string format, correct format is [#RRGGBBAA]";
		throw std::invalid_argument::invalid_argument
		("incorrect color string format, correct format is [#RRGGBBAA]");
	}

	// convert everything past the hashtag into a number and construct us from that
	unsigned int hex = std::atoi(&string.color[1]);

	*this = Color{hex32{hex}};
}

Engine::Color& Engine::Color::Mix(const Color& other, float scale) const
{
	return static_cast<Engine::Color>(glm::mix(static_cast<glm::vec4>(*this), static_cast<glm::vec4>(other), scale));
}

Engine::Color& Engine::Color::GetHSV() const
{
	if (this->mode == ColorMode::HSV)
		return Engine::Color{*this};

	return {glm::vec4{glm::hsvColor(glm::vec3{this->r, this->g, this->b}), 1.0f}};
}

Engine::Color& Engine::Color::GetRGB() const
{
	if (this->mode == ColorMode::RGB)
		return Engine::Color {*this};

	return {glm::vec4{glm::rgbColor(glm::vec3{this->r, this->g, this->b}), 1.0f}};
}

void Engine::Color::ConvertTo(ColorMode mode)
{
	if (this->mode == mode)
		return;

	if (this->mode == ColorMode::RGB)
	{
		Color& hsv = this->GetHSV();

		this->r = hsv.r;
		this->g = hsv.g;
		this->b = hsv.b;
		this->a = hsv.a;

		this->mode == ColorMode::HSV;
	}
	else
	{
		Color& rgb = this->GetRGB();

		this->r = rgb.r;
		this->g = rgb.g;
		this->b = rgb.b;
		this->a = rgb.a;

		this->mode == ColorMode::RGB;
	}
}

Engine::Color::operator glm::vec4()
{
	return {this->r, this->g, this->b, this->a};
}

Engine::Color::operator unsigned int()
{
	return ((static_cast<int>(this->r) * OxFF) << 24) +
		   ((static_cast<int>(this->g) * OxFF) << 16) +
		   ((static_cast<int>(this->b) * OxFF) << 8) +
		   ((static_cast<int>(this->a) * OxFF) << 0);
}

Engine::Color::operator std::string()
{
	return "#" + static_cast<unsigned int>(*this);
}
