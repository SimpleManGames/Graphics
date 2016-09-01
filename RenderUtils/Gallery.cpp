#include "Gallery.h"

int Gallery::AddShaderToGallery( const char * name, const char * v, const char * f ) {
	shaders[ name ] = ::makeShader( v, f );
	return true;
}

int Gallery::LoadShaderToGallery( const char * name, const char * v, const char * f ) {
	if( !shaders.count( name ) ) {
		shaders[ name ] = ::loadShader( v, f );
		if( shaders[ name ].handle == 0 ) {
			fprintf( stderr, "Shader %s didn't loaded.\n", name );
			shaders.erase( name );
		}
		return true;
	} else {
		fprintf( stderr, "Shader %s didn't loaded.\n", name );
		return false;
	}
}

int Gallery::AddObjectToGallery( const char * name, const Vertex * verts, size_t vSize, const unsigned * tris, size_t tSize ) {
	objects[ name ] = ::makeGeometry( verts, vSize, tris, tSize );

	return true;
}

int Gallery::LoadObjectFromGallery( const char * name, const char * path ) {
	objects[ name ] = ::loadOBJ( path );

	return true;
}

const Geometry & Gallery::getObject( const char * name ) {
	return objects[ name ];
}

const Shader & Gallery::getShader( const char * name ) {

	return shaders[ name ];
}

int Gallery::Initialize() {
	LoadShaderToGallery( "defaultShader", "../res/shader/defaultVert.txt"
						   , "../res/shader/defaultFrag.txt" );
	LoadObjectFromGallery( "Cube", "../res/models/cube.obj" );
	LoadObjectFromGallery( "Sphere", "../res/models/sphere.obj" );
	return true;
}

int Gallery::Exit() {
	for each( auto shader in shaders ) freeShader( shader.second );
	for each ( auto object in objects ) freeGeometry( object.second );
	return true;
}