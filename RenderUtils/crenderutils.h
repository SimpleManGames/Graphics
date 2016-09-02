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
Texture loadTexture( const char * path );
void freeTexture( Texture & );

void draw( const Shader &, const Geometry & );
void draw( const Shader &, const Geometry &, float time );
void draw( const Shader &, const Geometry &, const float * M, const float V[ 16 ], const float P[ 16 ] );
void draw( const Shader &, const Geometry &, const float * M, const float V[ 16 ], const float P[ 16 ], int i );
void draw( const Shader &, const Geometry &, const Texture & t, const float  M[ 16 ], const float V[ 16 ], const float P[ 16 ] );