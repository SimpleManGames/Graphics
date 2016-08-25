#include "window.h"

int main() {
	Window window;
	window.Initialize();
	while (window.Update());

	return window.Exit();
}