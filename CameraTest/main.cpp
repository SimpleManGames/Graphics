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
	camera.jumpTo( glm::vec3( 5, 5, 5 ) );
	camera.lookAt( glm::vec3( 0, 0, 0 ) );

	//gallery.LoadObjectFromGallery( "PandaMan", "../res/models/PandaMan.obj" );
	//gallery.LoadObjectFromGallery( "Fox", "../res/models/Fox.obj" );
	gallery.LoadObjectFromGallery( "SoulSpear", "../res/models/soulspear.obj" );

//	gallery.LoadShaderToGallery( "Texture", "../res/shader/texVert.txt"
	//							 , "../res/shader/texFrag.txt" );
	gallery.LoadShaderToGallery( "Phong", "../res/shader/phongVert.txt"
								 , "../res/shader/phongFrag.txt" );

	Texture tArray[ ] = { loadTexture( "../res/texture/soulspear_diffuse.tga" ), 
						  loadTexture( "../res/texture/soulspear_specular.tga" ),
						  loadTexture("../res/texture/soulspear_normal.tga" ) };

	Geometry plane = GenerateGrid( 50, 50 );

	glm::mat4 proj, view, model;

	int i = 1;

	while( window.Update() ) {
		time.Update();
		input.Update();

		view = camera.getView();
		proj = camera.getProj();

		camera.Update( input, time );

		draw( gallery.getShader( "Phong" ), gallery.getObject( "SoulSpear" ), glm::value_ptr( model ), glm::value_ptr( view ), glm::value_ptr( proj ), tArray, 3 );

		//draw( gallery.getShader( "Perlin" ), plane, tex, glm::value_ptr( model ), glm::value_ptr( view ), glm::value_ptr( proj ) );
	}

	gallery.Exit();
	time.Exit();
	input.Exit();
	return window.Exit();
}