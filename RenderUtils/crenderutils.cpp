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

#include "GLM\glm.hpp"
#include "GLM\ext.hpp"

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
	glEnableVertexAttribArray( 2 );
	glEnableVertexAttribArray( 3 );
	// describe the properties of the attribute (position)
	glVertexAttribPointer( 0, 4, GL_FLOAT, GL_FALSE, sizeof( Vertex ), ( void* )Vertex::POSITION );
	glVertexAttribPointer( 1, 4, GL_FLOAT, GL_FALSE, sizeof( Vertex ), ( void* )Vertex::COLOR );
	glVertexAttribPointer( 2, 4, GL_FLOAT, GL_FALSE, sizeof( Vertex ), ( void* )Vertex::NORMAL );
	glVertexAttribPointer( 3, 2, GL_FLOAT, GL_FALSE, sizeof( Vertex ), ( void* )Vertex::UV );
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

	Geometry retVal;

	for( int s = 0; s < shapes.size(); s++ ) {
		int vSize = shapes[ s ].mesh.indices.size();

		Vertex * verts = new Vertex[ vSize ];
		unsigned * tris = new unsigned[ vSize ];

		for( int i = 0; i < vSize; i++ ) {
			auto ind = shapes[ s ].mesh.indices[ i ];

			const float * n = &attrib.normals[ ind.normal_index * 3 ];
			const float * p = &attrib.vertices[ ind.vertex_index * 3 ];

			verts[ i ].position = glm::vec4( p[ 0 ], p[ 1 ], p[ 2 ], 1.0f );
			verts[ i ].normal = glm::vec4( n[ 0 ], n[ 1 ], n[ 2 ], 0.0f );

			if( ind.texcoord_index >= 0 ) {
				const float * t = &attrib.texcoords[ ind.texcoord_index * 2 ];
				verts[ i ].uv = glm::vec2( t[ 0 ], t[ 1 ] );
			}
			tris[ i ] = i;
		}
		retVal = makeGeometry( verts, vSize, tris, vSize );

		delete[ ] verts;
		delete[ ] tris;
	}

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

	//glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	//glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

	glBindTexture( GL_TEXTURE_2D, 0 );

	return retval;
}

Texture makeTextureF( unsigned width, unsigned height, unsigned format, const float *pixels ) {
	Texture retval = { 0, width, height, format };

	glGenTextures( 1, &retval.handle );
	glBindTexture( GL_TEXTURE_2D, retval.handle );
	//glTexImage2D( GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, pixels );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_R32F, width, height, 0, GL_RED, GL_FLOAT, pixels );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	//glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

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
Framebuffer makeFramebuffer( unsigned width, unsigned height, unsigned nColors ) {
	Framebuffer retVal = { 0, width, height, nColors };

	glGenFramebuffers( 1, &retVal.handle );
	glBindFramebuffer( GL_FRAMEBUFFER, retVal.handle );

	retVal.depth = makeTexture( width, height, GL_DEPTH_COMPONENT, 0 );
	glFramebufferTexture( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, retVal.depth.handle, 0 );

	const GLenum attachments[ 8 ] = {
		GL_COLOR_ATTACHMENT0,
		GL_COLOR_ATTACHMENT1,
		GL_COLOR_ATTACHMENT2,
		GL_COLOR_ATTACHMENT3,
		GL_COLOR_ATTACHMENT4,
		GL_COLOR_ATTACHMENT5,
		GL_COLOR_ATTACHMENT6,
		GL_COLOR_ATTACHMENT7
	};

	for( int i = 0; i < nColors; ++i ) {
		retVal.colors[ i ] = makeTexture( width, height, GL_RGBA, 0 );
		glFramebufferTexture( GL_FRAMEBUFFER, attachments[ i ], retVal.colors[ i ].handle, 0 );
	}

	glDrawBuffers( nColors, attachments );
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );

	return retVal;
}
void freeFramebuffer( Framebuffer & b ) {
	for( int i = 0; i < b.nColors; ++i ) {
		freeTexture( b.colors[ i ] );
	}
	glDeleteFramebuffers( 1, &b.handle );
	b = { 0,0,0,0 };
}

void clearFramebuffer( const Framebuffer & b ) {
	//glClearColor( .5f, .5f, .5f, 1.0f );
	glBindFramebuffer( GL_FRAMEBUFFER, b.handle );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
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
void draw( const Shader &s, const Geometry &g, const float * M, const float V[ 16 ], const float P[ 16 ], const Texture * T, unsigned t_count ) {
	glEnable( GL_CULL_FACE );
	glEnable( GL_DEPTH_TEST );

	glUseProgram( s.handle );
	glBindVertexArray( g.vao );

	glUniformMatrix4fv( 0, 1, GL_FALSE, P );
	glUniformMatrix4fv( 1, 1, GL_FALSE, V );
	glUniformMatrix4fv( 2, 1, GL_FALSE, M );

	for( int i = 0; i < t_count; ++i ) {
		glActiveTexture( GL_TEXTURE0 + i );
		glBindTexture( GL_TEXTURE_2D, T[ i ].handle );
		glUniform1i( 3 + i, 0 + i );
	}

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



	glDrawElements( GL_TRIANGLES, g.size, GL_UNSIGNED_INT, 0 );
}

void drawFB( const Shader & s, const Geometry & g, const Framebuffer & f, const float M[ 16 ], const float V[ 16 ], const float P[ 16 ], const Texture * T, unsigned t_count ) {
	glEnable( GL_CULL_FACE );
	glEnable( GL_DEPTH_TEST );

	glBindFramebuffer( GL_FRAMEBUFFER, f.handle );
	glUseProgram( s.handle );
	glBindVertexArray( g.vao );

	glViewport( 0, 0, f.width, f.height );

	glUniformMatrix4fv( 0, 1, GL_FALSE, P );
	glUniformMatrix4fv( 1, 1, GL_FALSE, V );
	glUniformMatrix4fv( 2, 1, GL_FALSE, M );

	for( int i = 0; i < t_count; ++i ) {
		glActiveTexture( GL_TEXTURE0 + i );
		glBindTexture( GL_TEXTURE_2D, T[ i ].handle );
		glUniform1i( 3 + i, 0 + i );
	}

	glDrawElements( GL_TRIANGLES, g.size, GL_UNSIGNED_INT, 0 );
}


void Draw_Internal::BeginDraw( const Shader & s, const Geometry & g, const Framebuffer & f ) {
	glBindFramebuffer( GL_FRAMEBUFFER, f.handle );
	glUseProgram( s.handle );
	glBindVertexArray( g.vao );

	glEnable( GL_CULL_FACE );
	glEnable( GL_DEPTH_TEST );
	glViewport( 0, 0, f.width, f.height );
}

void Draw_Internal::EndDraw( const Shader & s, const Geometry & g, const Framebuffer & f ) {

	glDrawElements( GL_TRIANGLES, g.size, GL_UNSIGNED_INT, 0 );

	glBindVertexArray( 0 );
	glUseProgram( 0 );
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );

}

size_t Draw_Internal::Format( size_t idx, size_t tex, int val ) {
	glUniform1i( idx, val );
	return 0;
}

size_t Draw_Internal::Format( size_t idx, size_t tex, float val ) {
	glUniform1f( idx, val );
	return 0;
}

size_t Draw_Internal::Format( size_t idx, size_t tex, const Texture & val ) {
	glActiveTexture( GL_TEXTURE0 + tex );
	glBindTexture( GL_TEXTURE_2D, val.handle );
	glUniform1i( idx, tex );
	return tex + 1;
}

size_t Draw_Internal::Format( size_t idx, size_t tex, const glm::vec3 & val ) {
	glUniform3fv( idx, 1, glm::value_ptr( val ) );
	return 0;
}

size_t Draw_Internal::Format( size_t idx, size_t tex, const glm::mat4 & val ) {
	glUniformMatrix4fv( idx, 1, GL_FALSE, glm::value_ptr( val ) );
	return 0;
}

size_t Draw_Internal::Format( size_t idx, size_t tex, const float val[ 16 ] ) {
	glUniformMatrix4fv( idx, 1, GL_FALSE, val );
	return 0;
}
