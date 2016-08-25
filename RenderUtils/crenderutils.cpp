#define GLEW_STATIC
#include "glew\glew.h"
#include "vertex.h"
#include <cstdio>
#include "crenderutils.h"

Geometry makeGeometry(Vertex * verts, size_t vsize, unsigned int * tris, size_t tsize)
{
	Geometry retval;
	retval.size = tsize;
	// define our variables
	glGenBuffers(1, &retval.vbo);
	glGenBuffers(1, &retval.ibo);
	glGenVertexArrays(1, &retval.vao);
	// scope our variables (ORDER MATTERS!)
	glBindVertexArray(retval.vao);
	glBindBuffer(GL_ARRAY_BUFFER, retval.vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, retval.ibo);
	// Load in our vertices
	glBufferData(GL_ARRAY_BUFFER, tsize * sizeof(Vertex), verts, GL_STATIC_DRAW);
	// Load in our triangles
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, tsize * sizeof(unsigned), tris,
		GL_STATIC_DRAW);
	// Active a vertex attribute (such as position)
	glEnableVertexAttribArray(0);
	// describe the properties of the attribute (position)
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	// Unscope our variables (ORDER MATTERS!)
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	return retval;
}

void freeGeometry(Geometry &geo)
{
	glDeleteBuffers(1, &geo.vbo);
	glDeleteBuffers(1, &geo.ibo);
	glDeleteVertexArrays(1, &geo.vao);
	geo = { 0,0,0,0 };
}