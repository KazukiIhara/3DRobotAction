#pragma once

// Windows
#include <windows.h>

// C++
#include <cstdint>

class DeltaTimer {
public:
	DeltaTimer();
	~DeltaTimer();

	void Reset();
	void Update();
	float GetDeltaTime() const;
private:
	// デルタタイム
	float deltaTime_;
	int64_t lastTime_;
	double frequency_;
};