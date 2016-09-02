#define GLEW_STATIC
#include "glew\glew.h"
#include "vertex.h"
#include <cstdio>
#include "crenderutils.h"
#include "filereaderhelper.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "OBJ\tiny_obj_loader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "STB\stb_image.h"

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
	//glEnableVertexAttribArray( 2 );
	// describe the properties of the attribute (position)
	glVertexAttribPointer( 0, 4, GL_FLOAT, GL_FALSE, sizeof( Vertex ), ( void* )Vertex::POSITION );
	glVertexAttribPointer( 1, 4, GL_FLOAT, GL_FALSE, sizeof( Vertex ), ( void* )Vertex::COLOR );
	//glVertexAttribPointer( 2, 1, GL_FLOAT, GL_FALSE, sizeof( Vertex ), ( void* )Vertex::SCALE );
	// Unscope our variables (ORDER MATTERS!)
	glBindVertexArray( 0 );
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
	retval.size = tsize;
	return retval;
}
Geometry loadOBJ( const char * path ) {
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string err;

	bool ret = tinyobj::LoadObj( &attrib, &shapes, &materials, &err, path );

	Vertex * verts = new Vertex[ attrib.vertices.size() / 3 ];
	unsigned * tris = new unsigned[ shapes[ 0 ].mesh.indices.size() ];

	for( int i = 0; i < attrib.vertices.size() / 3; ++i ) {
		verts[ i ] = {
			  attrib.vertices[ i * 3 ]
			, attrib.vertices[ i * 3 + 1 ]
			, attrib.vertices[ i * 3 + 2 ]
			, 1
		};
	}

	for( int i = 0; i < shapes[ 0 ].mesh.indices.size(); ++i ) {
		tris[ i ] = shapes[ 0 ].mesh.indices[ i ].vertex_index;
	}

	Geometry retVal = makeGeometry( verts, attrib.vertices.size() / 3,
									tris, shapes[ 0 ].mesh.indices.size() );

	delete[ ] verts;
	delete[ ] tris;

	return retVal;
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

	//Shader retval = makeShader( vSource, fSource );

	return makeShader( vSource, fSource );
}
void freeShader( Shader &shader ) {
	glDeleteProgram( shader.handle );
	shader.handle = 0;
}
Texture makeTexture( unsigned width, unsigned height, unsigned format, const unsigned char * pixels ) {
	Texture retval = { 0, width, height, format };

	glGenTextures( 1, &retval.handle );
	glBindTexture( GL_TEXTURE_2D, retval.handle );
	glTexImage2D( GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, pixels );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

	glBindTexture( GL_TEXTURE_2D, 0 );

	return retval;
}
Texture loadTexture( const char * path ) {
	Texture retval = { 0, 0, 0, 0 };

	stbi_set_flip_vertically_on_load( true );

	int w, h, f;
	unsigned char * p;

	p = stbi_load( path, &w, &h, &f, STBI_default );

	if( !p ) return retval;

	switch( f ) {
		case STBI_grey: f = GL_RED; break;
		case STBI_grey_alpha: f = GL_RG; break;
		case STBI_rgb: f = GL_RGB; break;
		case STBI_rgb_alpha: f = GL_RGBA; break;
	}
	retval = makeTexture( w, h, f, p );
	stbi_image_free( p );
	return retval;
}
void freeTexture( Texture &t ) {
	glDeleteTextures( 1, &t.handle );
	t = { 0,0,0,0 };
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

void draw( const Shader &shader, const Geometry &geo, float time ) {

	glUseProgram( shader.handle );
	// Binding the VAO also binds the IBO and VBO
	glBindVertexArray( geo.vao );

	// Wire Frame Mode
	//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

	// Set the uniform of 'time' inside the shader
	// Return -1 if false
	int loc = glGetUniformLocation( shader.handle, "time" );
	glUniform1f( loc, time );

	// Draw elements will draw the verts that are currently bound
	//using an array of indices
	// IF AN IBO IS BOUND, we don't need to provide any indices
	glDrawElements( GL_TRIANGLES, geo.size, GL_UNSIGNED_INT, 0 );
}

void draw( const Shader &s, const Geometry &g, const float * M, const float V[ 16 ], const float P[ 16 ] ) {
	glEnable( GL_CULL_FACE );

	glUseProgram( s.handle );
	glBindVertexArray( g.vao );

	glUniformMatrix4fv( 0, 1, GL_FALSE, P );
	glUniformMatrix4fv( 1, 1, GL_FALSE, V );
	glUniformMatrix4fv( 2, 1, GL_FALSE, M );

	glDrawElements( GL_TRIANGLES, g.size, GL_UNSIGNED_INT, 0 );
}

void draw( const Shader &s, const Geometry &g, const float * M, const float V[ 16 ], const float P[ 16 ], int i ) {
	glEnable( GL_CULL_FACE );

	glUseProgram( s.handle );
	glBindVertexArray( g.vao );

	glUniformMatrix4fv( 0, 1, GL_FALSE, P );
	glUniformMatrix4fv( 1, 1, GL_FALSE, V );
	glUniformMatrix4fv( 2, 1, GL_FALSE, M );
	int loc = glGetUniformLocation( s.handle, "i" );
	glUniform1i( loc, i );

	glDrawElements( GL_TRIANGLES, g.size, GL_UNSIGNED_INT, 0 );
}

void draw( const Shader &s, const Geometry &g, const Texture & t, const float  M[ 16 ], const float V[ 16 ], const float P[ 16 ] ) {
	glEnable( GL_CULL_FACE );
	glEnable( GL_DEPTH_TEST );
	glUseProgram( s.handle );
	glBindVertexArray( g.vao );

	glUniformMatrix4fv( 0, 1, GL_FALSE, P );
	glUniformMatrix4fv( 1, 1, GL_FALSE, V );
	glUniformMatrix4fv( 2, 1, GL_FALSE, M );

	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, t.handle );
	int loc = glGetUniformLocation( s.handle, "texMap" );
	glUniform1i( loc, 0 );

	glDrawElements( GL_TRIANGLES, g.size, GL_UNSIGNED_INT, 0 );
}
