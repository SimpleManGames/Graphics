#include "GLFW/glfw3.h"
#include "Input.h"
#include "window.h"
#include <cstring>
#include <cstdio>

bool Input::Initialize( const Window & w ) {
	memset( keys, 0, 350 * sizeof( KEY_STATE ) );
	winHandle = w.winHandle;
	glfwSetInputMode( winHandle, GLFW_CURSOR, GLFW_CURSOR_DISABLED );

	mouseXCur = mouseXPrev = mouseYCur = mouseYPrev = mouse_Hor = mouse_Ver = 0;

	return true;
}

bool Input::Update() {
	for( int i = 0; i < 350; ++i ) {
		int state = glfwGetKey( winHandle, i );

		if( ( keys[ i ] == UP || keys[ i ] == RELEASE ) && state == GLFW_PRESS ) { keys[ i ] = PRESS; } else if( ( keys[ i ] == DOWN || keys[ i ] == PRESS ) && state == GLFW_RELEASE ) { keys[ i ] = RELEASE; } else if( keys[ i ] == PRESS ) { keys[ i ] = DOWN; } else if( keys[ i ] == RELEASE ) { keys[ i ] = UP; }
	}

	int w, h;

	glfwGetWindowSize( winHandle, &w, &h );

	mouseXPrev = mouseXCur;
	mouseYPrev = mouseYCur;

	glfwGetCursorPos( winHandle, &mouseXCur, &mouseYCur );
	//glfwSetCursorPos( winHandle, w / 2, h / 2 );

	mouse_Hor = mouseXCur - mouseXPrev; 
	mouse_Ver = mouseYCur - mouseYPrev;

	return true;
}

bool Input::Exit() {
	return true;
}