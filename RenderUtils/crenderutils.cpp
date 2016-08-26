#define GLEW_STATIC
#include "glew\glew.h"
#include "vertex.h"
#include <cstdio>
#include "crenderutils.h"
#include "filereaderhelper.h"

Geometry makeGeometry( const Vertex * verts, size_t vsize
					   , const unsigned int * tris, size_t tsize ) {

	Geometry retval;
	retval.size = tsize;
	// define our variables
	glGenBuffers( 1, &retval.vbo );
	glGenBuffers( 1, &retval.ibo );
	glGenVertexArrays( 1, &retval.vao );
	// scope our variables (ORDER MATTERS!)
	glBindVertexArray( retval.vao );
	glBindBuffer( GL_ARRAY_BUFFER, retval.vbo );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, retval.ibo );
	// Load in our vertices
	glBufferData( GL_ARRAY_BUFFER, vsize * sizeof( Vertex ), verts, GL_STATIC_DRAW );
	// Load in our triangles
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, tsize * sizeof( unsigned ), tris,
				  GL_STATIC_DRAW );
			  // Active a vertex attribute (such as position)
	glEnableVertexAttribArray( 0 );
	glEnableVertexAttribArray( 1 );
	// describe the properties of the attribute (position)
	glVertexAttribPointer( 0, 4, GL_FLOAT, GL_FALSE, sizeof( Vertex ), ( void* )Vertex::POSITION );
	glVertexAttribPointer( 1, 4, GL_FLOAT, GL_FALSE, sizeof( Vertex ), ( void* )Vertex::COLOR );
	// Unscope our variables (ORDER MATTERS!)
	glBindVertexArray( 0 );
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
	retval.size = tsize;
	return retval;
}

void freeGeometry( Geometry &geo ) {

	glDeleteBuffers( 1, &geo.vbo );
	glDeleteBuffers( 1, &geo.ibo );
	glDeleteVertexArrays( 1, &geo.vao );
	geo = { 0,0,0,0 };
}

Shader makeShader( const char * vert, const char * frag ) {

	// Create varibles
	Shader retval;

	retval.handle = glCreateProgram();

	unsigned vs = glCreateShader( GL_VERTEX_SHADER );
	unsigned fs = glCreateShader( GL_FRAGMENT_SHADER );

	// Init varibles
	glShaderSource( vs, 1, &vert, 0 );
	glShaderSource( fs, 1, &frag, 0 );

	// Compile shaders
	glCompileShader( vs );
	glCompileShader( fs );

	// Link the shaders to one program
	glAttachShader( retval.handle, vs );
	glAttachShader( retval.handle, fs );
	glLinkProgram( retval.handle );

	// Delete the shader, don't need them anymore
	glDeleteShader( vs );
	glDeleteShader( fs );

	// Return the program
	return retval;
}

Shader loadShader( const char * vPath, const char * fPath ) {

	char* vSource = getStringFromFile( vPath );
	char* fSource = getStringFromFile( fPath );

	Shader retval = makeShader( vSource, fSource );

	return makeShader(vSource, fSource);
}

void freeShader( Shader &shader ) {
	glDeleteProgram( shader.handle );
	shader.handle = 0;
}

void draw( const Shader &shader, const Geometry &geo ) {

	glUseProgram( shader.handle );
	// Binding the VAO also binds the IBO and VBO
	glBindVertexArray( geo.vao );

	// Draw elements will draw the verts that are currently bound
	//using an array of indices
	// IF AN IBO IS BOUND, we don't need to provide any indices
	glDrawElements( GL_TRIANGLES, geo.size, GL_UNSIGNED_INT, 0 );
}