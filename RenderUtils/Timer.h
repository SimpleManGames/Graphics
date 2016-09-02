#pragma once

class Timer {
public:
	int Initialize();
	int Update();
	int Exit();

	float getDeltaTime() const;
	float getTotalTime() const;

	float getRealTime() const;

private:
	float currentTime;
	float prevTime;
	float deltaTime;
};