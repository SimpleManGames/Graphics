#include "window.h"
#include "Gallery.h"
#include "crenderutils.h"
#include "vertex.h"
#include "Timer.h"

int main() {
	Window window;
	window.Initialize();
	Gallery gallery;
	gallery.Initialize();

	Vertex verts[ 4 ] =
	{ { { -1, -1, 0, 1 },{},{},{ 0, 0 } },
	{ { 1, -1, 0, 1 },{},{},{ 1, 0 } },
	{ { 1,  1, 0, 1 },{},{},{ 1, 1 } },
	{ { -1,  1, 0, 1 },{},{},{ 0, 1 } } };

	unsigned tris[ ] = { 0, 1, 2, 2, 3, 0 };

	Geometry quad = makeGeometry( verts, 4, tris, 6 );

	Framebuffer screen = { 0, window.GetWidth(), window.GetHeight() };

	Texture img = loadTexture( "../res/texture/bigmisssteak.png" );

	float time = 0;
	while( window.Update() ) {
		time += 0.016f;
		Draw( gallery.getShader( "defaultShader" ), quad, screen );
	}

	gallery.Exit();
	return window.Exit();
}