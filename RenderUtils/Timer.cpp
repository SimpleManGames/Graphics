#include "Timer.h"
#include "GLFW/glfw3.h"

int Timer::Initialize() {
	glfwSetTime(0.0);

	return true;
}

int Timer::Step() {

	prevTime = currentTime;
	currentTime = getRealTime();

	deltaTime = currentTime - prevTime;

	return true;
}

int Timer::Term() {

	return true;
}

float Timer::getDeltaTime() const { return deltaTime; }
float Timer::getTotalTime() const { return currentTime; }
float Timer::getRealTime() const { return glfwGetTime(); }