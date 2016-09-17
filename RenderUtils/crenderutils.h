#pragma once
#include "globjects.h"
#include <cstring>
#include "GLM\fwd.hpp"

Geometry makeGeometry( const struct Vertex *verts, size_t vsize
					   , const unsigned int *tris, size_t tsize );
Geometry loadOBJ( const char * path );
void freeGeometry( Geometry & );

Shader makeShader( const char *vert, const char *frag );
Shader loadShader( const char *vPath, const char *fPath );
void freeShader( Shader & );

Texture makeTexture( unsigned width, unsigned height, unsigned format, const unsigned char * pixels );
Texture makeTextureF( unsigned width, unsigned height, unsigned format, const float *pixels );
Texture loadTexture( const char * path );
void freeTexture( Texture & );

Framebuffer makeFramebuffer( unsigned width, unsigned height, unsigned nColors );
void freeFramebuffer( Framebuffer & b );
void clearFramebuffer( const Framebuffer & b );

void draw( const Shader &, const Geometry & );
void draw( const Shader &, const Geometry &, float time );
void draw( const Shader &s, const Geometry &g, const float * M, const float V[ 16 ], const float P[ 16 ], const Texture * T, unsigned t_count );
void draw( const Shader &, const Geometry &, const Texture & t, const float  M[ 16 ], const float V[ 16 ], const float P[ 16 ] );
void drawFB( const Shader &s, const Geometry &g, const Framebuffer &f, const float M[ 16 ], const float V[ 16 ], const float P[ 16 ], const Texture * T, unsigned t_count );

namespace Draw_Internal {
	void BeginDraw( const Shader &s, const Geometry &g, const Framebuffer &r );
	void EndDraw( const Shader &s, const Geometry &g, const Framebuffer &r );

	size_t Format( size_t idx, size_t tex, int val );
	size_t Format( size_t idx, size_t tex, float val );
	size_t Format( size_t idx, size_t tex, const Texture & val );
	size_t Format( size_t idx, size_t tex, const glm::vec3 & val );
	size_t Format( size_t idx, size_t tex, const glm::mat4 & val );
	size_t Format( size_t idx, size_t tex, const float val[ 16 ] );

	template<typename T, typename ...U>
	void Unpack( size_t idx, size_t tex, T val, U &&...uniforms ) {
		tex += Format( idx, tex, val );
		Unpack( idx + 1, tex, uniforms... );
	}

	template<typename T>
	void Unpack( size_t idx, size_t tex, T val ) {
		Format( idx, tex, val );
	}

}

template<typename ...U>
void Draw( const Shader &s, const Geometry &g, const Framebuffer &r, U ... uniforms ) {
	Draw_Internal::BeginDraw( s, g, r );

	Draw_Internal::Unpack( 0, 0, uniforms... ); // {4,3,2,1,2,4}

	Draw_Internal::EndDraw( s, g, r );
}



inline void Draw( const Shader &s, const Geometry &g, const Framebuffer &r ) {
	Draw_Internal::BeginDraw( s, g, r );
	Draw_Internal::EndDraw( s, g, r );
}