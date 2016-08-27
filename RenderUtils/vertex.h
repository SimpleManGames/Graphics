#pragma once

struct Vertex {

	float position[4];
	float color[4];
	float scale;

	enum { POSITION = 0, COLOR = 16, SCALE = 32 };
};