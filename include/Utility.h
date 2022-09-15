#pragma once

#include "glm/glm.hpp"

namespace Engine
{
	using Point = glm::vec3;
	
	// missing glm::vec3 comparison operators
	inline bool operator>(Point& A, Point& B) { return glm::all(glm::greaterThan(A, B)); };
	inline bool operator<(Point& A, Point& B) { return glm::all(glm::lessThan(A, B)); };

	inline bool operator<=(Point& A, Point& B) { return glm::all(glm::lessThanEqual(A, B)); };
	inline bool operator>=(Point& A, Point& B) { return glm::all(glm::greaterThanEqual(A, B)); };
}
