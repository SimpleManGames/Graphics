#include "window.h"
#include "Gallery.h"
#include "crenderutils.h"
#include "vertex.h"
#include <GLM/glm.hpp>
#include <GLM/ext.hpp>
#include "Timer.h"
#include "Input.h"
#include "Camera.h"
#include "MakeGeometry.h"

int main() {
	Window window;
	window.Initialize( 1280, 720 );
	Gallery gallery;
	gallery.Initialize();
	Timer time;
	time.Initialize();
	Input input;
	input.Initialize( window );

	unsigned char pixels[ ] = { 255, 255, 0 };
	Texture work = makeTexture( 1, 1, 0x1907, pixels );
	Texture tex = loadTexture( "../res/texture/bigmisssteak.png" );
	//Texture tex = PerlinNoise( 64, 8 );
	FlyCamera camera;
	camera.jumpTo( glm::vec3( -5, -5, -5 ) );
	camera.lookAt( glm::vec3( 0, 0, 0 ) );

	//gallery.LoadObjectFromGallery( "PandaMan", "../res/models/PandaMan.obj" );
	//gallery.LoadObjectFromGallery( "Fox", "../res/models/FoxM.obj" );
	gallery.LoadObjectFromGallery( "SoulSpear", "../res/models/soulspear.obj" );

	//gallery.LoadShaderToGallery( "Texture", "../res/shader/texVert.txt"
	//							 , "../res/shader/texFrag.txt" );
	gallery.LoadShaderToGallery( "Phong", "../res/shader/phongVert.vert"
								 , "../res/shader/phongFrag.frag" );

	//gallery.LoadShaderToGallery( "Depth", "../res/shader/depthVert.vert"
		//								, "../res/shader/depthFrag.frag" );

	Texture soulspear_diffuse = loadTexture( "../res/texture/soulspear_diffuse.tga" );
	Texture soulspear_specular = loadTexture( "../res/texture/soulspear_specular.tga" );
	Texture soulspear_normal = loadTexture( "../res/texture/soulspear_normal.tga" );

	Framebuffer frame = makeFramebuffer( 1280, 720, 3 );
	Framebuffer screen = { 0, 1280, 720, 1 };

	frame.colors[ frame.nColors ] = frame.depth;

	Vertex verts[ 4 ] =
	{ { { -1, -1, 0, 1 }, {}, {}, { 0, 0 } },
	  { {  1, -1, 0, 1 }, {}, {}, { 1, 0 } },
	  { {  1,  1, 0, 1 }, {}, {}, { 1, 1 } },
	  { { -1,  1, 0, 1 }, {}, {}, { 0, 1 } } };

	unsigned tris[ ] = { 0, 1, 2, 2, 3, 0 };

	Geometry quad = makeGeometry( verts, 4, tris, 6 );
	Shader post = loadShader( "../res/shader/postVert.vert",
							  "../res/shader/postFrag.frag" );

	glm::mat4 proj, view, model;
	view = camera.getView();

	while( window.Update() ) {
		time.Update();
		input.Update();

		clearFramebuffer( frame );

		view = camera.getView();
		proj = camera.getProj();

		camera.Update( input, time );

		Draw( gallery.getShader( "defaultShader" ), gallery.getObject( "SoulSpear" ), screen, soulspear_diffuse, soulspear_normal, soulspear_specular );
		//Draw( post, quad, screen, glm::mat4(), glm::mat4(), glm::mat4(), frame.colors, frame.nColors + 1 );
	}

	freeFramebuffer( frame );
	gallery.Exit();
	time.Exit();
	input.Exit();
	return window.Exit();
}