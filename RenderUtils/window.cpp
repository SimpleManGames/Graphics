//window.cpp
#define GLEW_STATIC
#include "glew\glew.h"
#include "glfw\glfw3.h"
#include <cstring>
#include "window.h"
#include <cstdio>
#include <cstdlib>

#ifdef _DEBUG
void APIENTRY GL_errorCallback(GLenum source, GLenum type, 
	GLuint id, GLenum severity, 
	GLsizei length, const GLchar *message, 
	const void *userParam) {

	fprintf(stderr, "%s\n", message);

	if (severity == GL_DEBUG_SEVERITY_HIGH) abort();
}
#endif

int Window::Initialize(int a_width, int a_height, char * a_title)
{
	width = a_width;
	height = a_height;
	strcpy_s(title, 64, a_title);

	glfwInit();
	winHandle = glfwCreateWindow(width, height, title, nullptr, nullptr);
	glfwMakeContextCurrent(winHandle);
	
	glewExperimental = true;
	glewInit();
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

	isInit = true;

#ifdef _DEBUG
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

	glDebugMessageCallback(GL_errorCallback, 0);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, 0, true);
#endif

	return true;
}

int Window::Update()
{
	if (!isInit) return false;

	glfwMakeContextCurrent( winHandle );
	glfwPollEvents();
	glfwSwapBuffers( winHandle );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	return !glfwWindowShouldClose(winHandle);
}

int Window::Exit()
{
	glfwDestroyWindow(winHandle);
	glfwTerminate();
	winHandle = nullptr;
	isInit = false;

	return true;
}