#pragma once

#include <string>
#include <map>
#include "crenderutils.h"

struct Name { char data[ 64 ]; };

class Gallery {
private:
	std::map<std::string, Geometry> objects;
	std::map<std::string, Shader> shaders;
public:
	int AddShaderToGallery( const char * name, const char * v, const char * f );
	int LoadShaderFromGallery( const char * name, const char * v, const char * f );

	int AddObjectToGallery( const char * name, const Vertex * verts, size_t vSize, const unsigned * tris, size_t tSize );
	int LoadObjectFromGallery( const char * name, const char * path );

	const Geometry& getObject( const char * name );
	const Shader& getShader( const char * name );

	int Initialize();
	int Exit();
};