#include "window.h"
#include "crenderutils.h"
#include "vertex.h"

int main() {
	Window window;
	window.Initialize();

	Vertex vert[ 3 ] = {
		{	 0,  .5f, 0, 1, 1, 0, 0, 1 },
		{  .5f, -.5f, 0, 1, 0, 1, 0, 1 },
		{ -.5f, -.5f, 0, 1, 0, 0, 1, 1 }
	};

	unsigned tris[ 3 ] = { 0, 1, 2 };

	const char vertSha[ ] =
		"#version 450\n"
		"layout(location = 0)in vec4 position;"
		"layout(location = 1)in vec4 color;"
		"out vec4 vColor;"
		"void main() { gl_Position = position; vColor = color; } ";
	const char fragSha[ ] =
		"#version 450\n"
		"in vec4 vColor;"
		"out vec4 outColor;"
		"void main() { outColor = vColor; } ";

	Geometry geo = makeGeometry( vert, 3, tris, 3 );
	Shader shader = makeShader( vertSha, fragSha );

	Shader shaderLoadTest = loadShader( "../res/shader/defaultVert.txt", "../res/shader/defaultFrag.txt" );

	while( window.Update() ) {
		draw( shaderLoadTest, geo );
	}

	freeGeometry( geo );
	freeShader( shader );
	freeShader( shaderLoadTest );

	return window.Exit();
}