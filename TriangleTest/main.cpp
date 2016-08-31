#include "window.h"
#include "Gallery.h"
#include "crenderutils.h"
#include "vertex.h"

int main() {
	Window window;
	window.Initialize();
	Gallery gallery;
	gallery.Initialize();
	float time = 0.0f;
	while( window.Update() ) {
		time += 0.01666f;
		draw( gallery.getShader( "defaultShader" ), gallery.getObject( "Cube" ), time );
	}

	gallery.Exit();
	return window.Exit();
}