#ifndef MAIN_H
#define MAIN_H

#define STB_IMAGE_IMPLEMENTATION
#define OBJL_INCLUDED

#ifndef __glad_h_
#include "glad/glad.h"
#define __glad_h_
#endif

#include "glfw3.h"
#include "stb_image.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/string_cast.hpp>
// #include "Shader.h"
#include "Material.h"
#include "window.h"
#include "Engine.h"
// #include "Model.h"
#include "Object.h"
#include "Actor.h"
#include "Camera.h"
#include "BoundingVolume.h"
#include "Skybox.h"
#include "Terrain.h"
#include "FrameBuffer.h"
#include "Shared.h"

#include "fstream"
#include "math.h"
#include "random"
#include "chrono"

namespace Engine
{

void ErrorCallback(int error, const char* description);

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

void FrameBufferCallback(GLFWwindow* window, int width, int height);

void MouseCallback(GLFWwindow* window, double xPos, double yPos);

void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset);

void InitializeCallbacks(Motor* Motor);

int Initialize();
}

#endif //  MAIN_H
