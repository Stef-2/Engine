#ifndef LIGHT_H
#define LIGHT_H

#include "Object.h"

namespace Engine
{
	class Light : public Engine::Object
	{
		using Engine::Object::Object;

		float intensity;
		glm::vec3 color;

	};
}
#endif
