#pragma once
#include "vertex.h"
#include "globjects.h"
#include "crenderutils.h"
#include "GLM\glm.hpp"

Geometry GenerateGrid( unsigned int rows, unsigned int cols ) {
	Vertex * aoVert = new Vertex[ rows * cols ];
	for( unsigned int r = 0; r < rows; ++r )
		for( unsigned int c = 0; c < cols; ++c ) {
			aoVert[ r * cols + c ].position = glm::vec4( ( float )c/* / ( ( float )cols - 1 )*/, 
														 0, ( float )r /*/ ( ( float )rows - 1 )*/, 1 );

			glm::vec3 color = glm::vec3( sinf( ( c / ( float )( cols - 1 ) )
											   * ( r / ( float )( rows - 1 ) ) ) );
			aoVert[ r * cols + c ].color = glm::vec4( color, 1 );
		}

	unsigned int * auiIndices = new unsigned int[ ( rows - 1 ) * ( cols - 1 ) * 6 ];

	unsigned int index = 0;

	for( unsigned int r = 0; r < ( rows - 1 ); ++r )
		for( unsigned int c = 0; c < ( cols - 1 ); ++c ) {
			auiIndices[ index++ ] = r * cols + c;
			auiIndices[ index++ ] = ( r + 1 ) * cols + c;
			auiIndices[ index++ ] = ( r + 1 ) * cols + ( c + 1 );

			auiIndices[ index++ ] = r * cols + c;
			auiIndices[ index++ ] = ( r + 1 ) * cols + (c + 1);
			auiIndices[ index++ ] =  r * cols + ( c + 1 );
		}

	Geometry g = makeGeometry( aoVert, rows * cols, auiIndices, ( rows - 1 ) * ( cols - 1 ) * 6 );
	delete[ ] aoVert;
	return g;
}