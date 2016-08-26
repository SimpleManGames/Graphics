#pragma once

struct Geometry {

	unsigned vbo, ibo, vao, size;
	// Vertex Buffer Object : an array of vertices
	// Index Buffer Object  : an array of indices (triangles)
	// Vertex Array Object  : Groups the two with some formatting
	// size                 : number of triangles.
};

Geometry makeGeometry( const struct Vertex *verts, size_t vsize
					   , const unsigned int *tris, size_t tsize );

void freeGeometry( Geometry & );

struct Shader { unsigned handle; };

Shader makeShader( const char *vert, const char *frag );

Shader loadShader( const char *vPath, const char *fPath );
void freeShader( Shader & );

void draw( const Shader &, const Geometry & );