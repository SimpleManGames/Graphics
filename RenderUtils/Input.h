#pragma once

class Input {
public:
	enum KEY_STATE { UP, DOWN, PRESS, RELEASE };
private:
	struct GLFWwindow *winHandle;
	KEY_STATE keys[ 350 ];

	double mouseXCur, mouseYCur, mouseXPrev, mouseYPrev;

	float mouse_Hor, mouse_Ver;

public:
	bool Initialize( const class Window &w );
	bool Update();
	bool Exit();

	KEY_STATE getKeyState( int key ) const { return keys[ key ]; }

	float getMouseHorz() const { return mouse_Hor; }
	float getMouseVer() const { return mouse_Ver; }
};