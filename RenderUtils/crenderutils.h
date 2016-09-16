#pragma once
#include "globjects.h"

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

Framebuffer makeFramebuffer(unsigned width, unsigned height, unsigned nColors);
void freeFramebuffer( Framebuffer & b );
void clearFramebuffer( const Framebuffer & b );

void draw( const Shader &, const Geometry & );
void draw( const Shader &, const Geometry &, float time );
void draw( const Shader &s, const Geometry &g, const float * M, const float V[ 16 ], const float P[ 16 ], const Texture * T, unsigned t_count );
void draw( const Shader &, const Geometry &, const Texture & t, const float  M[ 16 ], const float V[ 16 ], const float P[ 16 ] );
void drawFB( const Shader &s, const Geometry &g, const Framebuffer &f, const float M[16], const float V[ 16 ], const float P[ 16 ], const Texture * T, unsigned t_count );