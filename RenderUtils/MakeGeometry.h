#pragma once
#include "vertex.h"
#include "globjects.h"
#include "crenderutils.h"
#include "GLM\glm.hpp"

Geometry GenerateGrid( unsigned int rows, unsigned int cols ) {
	Vertex * aoVert = new Vertex[ rows * cols ];
	for( unsigned int r = 0; r < rows; ++r )
		for( unsigned int c = 0; c < cols; ++c ) {
			aoVert[ r * cols + c ].position = glm::vec4( ( float )c, 0, ( float )r, 1 );

			aoVert[ r * cols + c ].uv = glm::vec2( c * 1.F / ( float )( cols - 1 ),
												   r * 1.F / ( float )( rows - 1 ) );

			glm::vec3 color = glm::vec3( sinf( ( c / ( float )( cols - 1 ) )
											   * ( r / ( float )( rows - 1 ) ) ) );

			//aoVert[ r * cols + c ].color = glm::vec4( color, 1 );
		}

	unsigned int * auiIndices = new unsigned int[ ( rows - 1 ) * ( cols - 1 ) * 6 ];

	unsigned int index = 0;

	for( unsigned int r = 0; r < ( rows - 1 ); ++r )
		for( unsigned int c = 0; c < ( cols - 1 ); ++c ) {
			auiIndices[ index++ ] = r * cols + c;
			auiIndices[ index++ ] = ( r + 1 ) * cols + c;
			auiIndices[ index++ ] = ( r + 1 ) * cols + ( c + 1 );

			auiIndices[ index++ ] = r * cols + c;
			auiIndices[ index++ ] = ( r + 1 ) * cols + ( c + 1 );
			auiIndices[ index++ ] = r * cols + ( c + 1 );
		}

	Geometry g = makeGeometry( aoVert, rows * cols, auiIndices, ( rows - 1 ) * ( cols - 1 ) * 6 );
	delete[ ] aoVert;
	return g;
}

Texture PerlinNoise( int size = 64, int oct = 6 ) {
	float * perlinData = new float[ size * size ];
	float scale = ( 1.0f / size ) * 3;

	for( int x = 0; x < size; ++x ) {
		for( int y = 0; y < size; ++y ) {
			float ampl = 1.f;
			float pers = .3f;
			perlinData[ y * size + x ] = 0;

			for( int o = 0; o < oct; ++o ) {
				float freq = powf( 2, ( float )o );
				float perlinSample = glm::perlin( 
					glm::vec2( (float)x, (float)y ) * scale * freq) * 0.5f + 0.5f;
				perlinData[ y * size + x ] += perlinSample * ampl;
				ampl *= pers;
			}
		}
	}

	return makeTextureF( size, size, 0x1907, perlinData );
}