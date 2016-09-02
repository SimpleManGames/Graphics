#include "window.h"
#include "Gallery.h"
#include "crenderutils.h"
#include "vertex.h"
#include <GLM/glm.hpp>
#include <GLM/ext.hpp>
#include "Timer.h"
#include "Input.h"
#include "Camera.h"

int main() {
	Window window;
	window.Initialize( 1280, 720 );
	Gallery gallery;
	gallery.Initialize();
	Timer time;
	time.Initialize();
	Input input;
	input.Initialize( window );

	FlyCamera camera;
	camera.jumpTo( glm::vec3( 5, 5, 5 ) );
	camera.lookAt( glm::vec3( 0, 0, 0 ) );

	gallery.LoadObjectFromGallery( "PandaMan", "../res/models/PandaMan.obj" );
	gallery.LoadObjectFromGallery( "Fox", "../res/models/Fox.obj" );

	gallery.LoadShaderToGallery( "Camera", "../res/shader/camVert.txt"
								 , "../res/shader/camFrag.txt" );

	glm::mat4 proj, view, model;

	float ct = 0;

	while( window.Update() ) {
		time.Update();
		input.Update();
		float cTime = time.getTotalTime();

		view = camera.getView();
		proj = camera.getProj();

		camera.Update( input, time );
		draw( gallery.getShader( "Camera" ), gallery.getObject( "Fox" ), glm::value_ptr( model ), glm::value_ptr( view ), glm::value_ptr( proj ) );
	}

	gallery.Exit();
	time.Exit();
	input.Exit();
	return window.Exit();
}