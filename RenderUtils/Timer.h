#pragma once

class Timer {
public:
	int Initialize();
	int Step();
	int Term();

	float getDeltaTime() const;
	float getTotalTime() const;

	float getRealTime() const;

private:
	float currentTime;
	float prevTime;
	float deltaTime;
};