#include "Random.h"
#include <random>

uint32_t Random::GenerateUint32_t(uint32_t min, uint32_t max) {
	static thread_local std::mt19937 generator(std::random_device{}());
	std::uniform_int_distribution<uint32_t> distribution(min, max);
	return distribution(generator);
}

int32_t Random::GenerateInt32_t(int32_t min, int32_t max) {
	static thread_local std::mt19937 generator(std::random_device{}());
	std::uniform_int_distribution<int32_t> distribution(min, max);
	return distribution(generator);
}

float Random::GenerateFloat(float min, float max) {
	static thread_local std::mt19937 generator(std::random_device{}());
	std::uniform_real_distribution<float> distribution(min, max);
	return distribution(generator);
}

double Random::GenerateDouble(double min, double max) {
	static thread_local std::mt19937 generator(std::random_device{}());
	std::uniform_real_distribution<double> distribution(min, max);
	return distribution(generator);
}

Vector2 Random::GenerateVector2(float min, float max) {
	return { GenerateFloat(min, max), GenerateFloat(min, max) };
}

Vector3 Random::GenerateVector3(float min, float max) {
	return { GenerateFloat(min, max), GenerateFloat(min, max), GenerateFloat(min, max) };
}

Vector4 Random::GenerateVector4(float min, float max) {
	return { GenerateFloat(min, max), GenerateFloat(min, max), GenerateFloat(min, max), GenerateFloat(min, max) };
}

float Random::GenerateSeed1Sec() {
	using namespace std::chrono;

	auto now = system_clock::now();
	time_t now_t = system_clock::to_time_t(now);

	std::tm localTm;

	localtime_s(&localTm, &now_t);

	uint32_t secondsSinceMidnight =
		static_cast<uint32_t>(localTm.tm_hour * 3600 +
			localTm.tm_min * 60 +
			localTm.tm_sec);

	return static_cast<float>(secondsSinceMidnight);
}
