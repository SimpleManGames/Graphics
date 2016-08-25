#include "window.h"
#include "crenderutils.h"
#include "vertex.h"

int main() {
	Window window;
	window.Initialize();

	Vertex vert[3] = { 
		{	 0,  .5f, 0, 1 }, 
		{  .5f, -.5f, 0, 1 }, 
		{ -.5f, -.5f, 0, 1 } 
	};
	unsigned tris[3] = { 0, 1, 2 };

	const char vertSha[] =
		"#version 150\n"
		"in vec4 position;"
		"void main() { gl_Position = position; } ";
	const char fragSha[] =
		"#version 150\n"
		"out vec4 outColor;"
		"void main() { outColor = vec4(0.0, 1.0, 0.0, 1.0); } ";

	Geometry geo = makeGeometry(vert, 3, tris, 3);
	Shader shader = makeShader(vertSha, fragSha);

	while (window.Update()) {

		draw(shader, geo);
	}

	freeGeometry(geo);
	freeShader(shader);

	return window.Exit();
}