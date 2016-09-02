#pragma once

class Window {
	friend class Input;
private:
	int width, height;
	char title[64];
	bool isInit = false;
	struct GLFWwindow *winHandle;

public:
	int Initialize(int a_width = 600, int a_height = 600, char *a_title = "Title");
	int Update();
	int Exit();
};