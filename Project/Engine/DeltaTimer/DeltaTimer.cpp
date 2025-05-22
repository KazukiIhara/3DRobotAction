#include "DeltaTimer.h"

#include "Logger/Logger.h"

DeltaTimer::DeltaTimer() {
	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);
	frequency_ = static_cast<double>(freq.QuadPart);
	Reset();

	Logger::Log("DeltaTimer Initialize\n");
}

DeltaTimer::~DeltaTimer() {
	Logger::Log("DeltaTimer Finalize\n");
}

void DeltaTimer::Reset() {
	LARGE_INTEGER startTime;
	QueryPerformanceCounter(&startTime);
	lastTime_ = startTime.QuadPart;
}

void DeltaTimer::Update() {
	LARGE_INTEGER currentTime;
	QueryPerformanceCounter(&currentTime);

	const int64_t current = currentTime.QuadPart;
	const double deltaTime = static_cast<double>(current - lastTime_) / frequency_;

	lastTime_ = current;

	deltaTime_ = static_cast<float>(deltaTime);
}

float DeltaTimer::GetDeltaTime() const {
	return deltaTime_;
}
