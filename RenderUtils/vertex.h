#pragma once

#include "GLM\glm.hpp"

struct Vertex {
public:
	glm::vec4 position;
	glm::vec4 color;
	float scale;

	enum { POSITION = 0, COLOR = 16, SCALE = 32 };
};