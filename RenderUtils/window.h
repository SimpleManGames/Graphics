#pragma once

class Window {

private:
	int width, height;
	char title[64];
	bool isInit = false;
	struct GLFWwindow *winHandle;

public:
	int Initialize(int a_width = 800, int a_height = 600, char *a_title = "Title");
	int Update();
	int Exit();
};