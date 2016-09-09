#pragma once

#include "GLM\glm.hpp"

struct Vertex {
public:
	glm::vec4 position;
	glm::vec4 normal;
	glm::vec2 uv;

	enum { POSITION = 0, NORMAL = 16, UV = 32 };
};