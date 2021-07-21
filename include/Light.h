#ifndef LIGHT_H
#define LIGHT_H

#include "Object.h"

namespace Engine
{
	//base light class, defining common light attributes
	//it inherits transformation handling mechanisms from Engine::Object
	//it is to be inherited by more specialized lights
	class Light : public Engine::Object
	{
		using Engine::Object::Object;

		float intensity;
		glm::vec3 color;

	};
}
#endif
