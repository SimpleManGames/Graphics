//window.cpp
#define GLEW_STATIC
#include "glew\glew.h"
#include "glfw\glfw3.h"
#include <cstring>
#include "window.h"

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
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

	isInit = true;

	return true;
}

int Window::Update()
{
	if (!isInit) return false;

	glfwMakeContextCurrent(winHandle);
	glfwPollEvents();
	glfwSwapBuffers(winHandle);
	glClear(GL_COLOR_BUFFER_BIT);
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
