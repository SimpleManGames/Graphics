#include "window.h"
#include "Gallery.h"
#include "crenderutils.h"
#include "vertex.h"
#include <GLM/glm.hpp>
#include <GLM/ext.hpp>
#include "Timer.h"

int main() {
	Window window;
	window.Initialize();
	Gallery gallery;
	gallery.Initialize();
	Timer time;
	time.Initialize();
	gallery.LoadObjectFromGallery( "PandaMan", "../res/models/PandaMan.obj" );
	//gallery.LoadObjectFromGallery( "Fox", "../res/models/Fox.obj" );

	gallery.LoadShaderToGallery( "Camera", "../res/shader/camVert.txt"
								 , "../res/shader/camFrag.txt" );

	float INDENTITY[ 16 ] = {
						1, 0, 0, 0,
						0, 1, 0, 0,
						0, 0, 1, 0,
						0, 0, 0, 1
	};

	glm::mat4 proj, view, model;

	proj = glm::ortho<float>( -20, 20, -20, 20, -1000, 1000 );
	//proj = glm::perspective( 45.f, 1.f, .1f, 1000.f );
	model = glm::scale( glm::vec3( .5f, .5f, .5f ) ) * glm::translate( glm::vec3( .5f, .1f, .1f ) );
	view = glm::lookAt( glm::vec3( 5, 5, 5 ), glm::vec3( 0, 0, 0 ), glm::vec3( 0, 1, 0 ) );

	while( window.Update() ) {
		time.Step();
		float cTime = time.getTotalTime();

		model = glm::rotate( cTime, glm::vec3( 0, 1, 0 ) );

		//draw( gallery.getShader( "Camera" ), gallery.getObject( "Fox" ), glm::value_ptr( model * glm::translate( glm::vec3( .5, -.5, -.5 ) ) * glm::rotate( cTime, glm::vec3( 1, 0, 1 ) ) ), glm::value_ptr( view ), glm::value_ptr( proj ) );
		draw( gallery.getShader( "Camera" ), gallery.getObject( "PandaMan" ), glm::value_ptr( model * glm::translate( glm::vec3( .5, -.5, -.5 ) ) * glm::rotate( cTime, glm::vec3( 1, 0, 1 ) ) ), glm::value_ptr( view ), glm::value_ptr( proj ) );

		/*draw( gallery.getShader( "Camera" ), gallery.getObject( "Cube" ), glm::value_ptr( model ), glm::value_ptr( view ), glm::value_ptr( proj ) );
		draw( gallery.getShader( "Camera" ), gallery.getObject( "Cube" ), glm::value_ptr( model * glm::translate( glm::vec3( .5, .5, .5 ) ) * glm::rotate( time, glm::vec3( 1, 0, 1 ) ) ), glm::value_ptr( view ), glm::value_ptr( proj ) );
		draw( gallery.getShader( "Camera" ), gallery.getObject( "Cube" ), glm::value_ptr( model * glm::translate( glm::vec3( -.5, -.5, -.5 ) ) * glm::rotate( time, glm::vec3( 1, 0, 1 ) ) ), glm::value_ptr( view ), glm::value_ptr( proj ) );
		draw( gallery.getShader( "Camera" ), gallery.getObject( "Cube" ), glm::value_ptr( model * glm::translate( glm::vec3( -.5, -.5, .5 ) ) * glm::rotate( time, glm::vec3( 1, 0, 1 ) ) ), glm::value_ptr( view ), glm::value_ptr( proj ) );
		draw( gallery.getShader( "Camera" ), gallery.getObject( "Cube" ), glm::value_ptr( model * glm::translate( glm::vec3( .5, -.5, .5 ) ) * glm::rotate( time, glm::vec3( 1, 0, 1 ) ) ), glm::value_ptr( view ), glm::value_ptr( proj ) );
		draw( gallery.getShader( "Camera" ), gallery.getObject( "Cube" ), glm::value_ptr( model * glm::translate( glm::vec3( -.5, .5, -.5 ) ) * glm::rotate( time, glm::vec3( 1, 0, 1 ) ) ), glm::value_ptr( view ), glm::value_ptr( proj ) );
		draw( gallery.getShader( "Camera" ), gallery.getObject( "Cube" ), glm::value_ptr( model * glm::translate( glm::vec3( -.5, .5, .5 ) ) * glm::rotate( time, glm::vec3( 1, 0, 1 ) ) ), glm::value_ptr( view ), glm::value_ptr( proj ) );
		draw( gallery.getShader( "Camera" ), gallery.getObject( "Cube" ), glm::value_ptr( model * glm::translate( glm::vec3( .5, -.5, -.5 ) ) * glm::rotate( time, glm::vec3( 1, 0, 1 ) ) ), glm::value_ptr( view ), glm::value_ptr( proj ) );
		*/
		/*for( int i = 0; i < 50; i++ ) {
			draw( gallery.getShader( "Camera" ), gallery.getObject( "Cube" ), glm::value_ptr( model * glm::translate( glm::vec3( sinf( i * time ) / cosf( i * time ) * tan( sqrtf( time ) ), cosf( i * time ) / sinf( i * time ) * tan( sqrtf( time ) ), tanf( i * time ) / sinf( i * time ) * tan( sqrtf( time ) ) ) ) * glm::rotate( time, glm::vec3( 1, 0, 0 ) ) ), glm::value_ptr( view ), glm::value_ptr( proj ) );
			draw( gallery.getShader( "Camera" ), gallery.getObject( "Cube" ), glm::value_ptr( model * glm::translate( glm::vec3( sinf( i / time ) * cosf( i / time ) / tan( sqrtf( time ) ), cosf( i / time ) * sinf( i / time ) / tan( sqrtf( time ) ), tanf( i / time ) * sinf( i / time ) / tan( sqrtf( time ) ) ) ) * glm::rotate( time, glm::vec3( 1, 0, 0 ) ) ), glm::value_ptr( view ), glm::value_ptr( proj ) );
		}*/
	}

	gallery.Exit();
	return window.Exit();
}