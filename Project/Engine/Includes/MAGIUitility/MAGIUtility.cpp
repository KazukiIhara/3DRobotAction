#include "MAGIUtility.h"

#include <cassert>

using namespace MAGIMath;

Vector3 MAGIUtility::CalculateVelue(const std::vector<KeyframeVector3>& keyframes, float time) {
	assert(!keyframes.empty());

	if (keyframes.size() == 1 || time <= keyframes[0].time) {
		return keyframes[0].value;
	}

	for (size_t index = 0; index < keyframes.size() - 1; ++index) {
		size_t nextIndex = index + 1;
		// indexとnextIndexの2つのKeyframeを取得して範囲内に時刻があるかを判定
		if (keyframes[index].time <= time && time <= keyframes[nextIndex].time) {
			// 範囲内を補完する
			float t = (time - keyframes[index].time) / (keyframes[nextIndex].time - keyframes[index].time);
			return Lerp(keyframes[index].value, keyframes[nextIndex].value, t);
		}
	}

	return (*keyframes.rbegin()).value;
}

Quaternion MAGIUtility::CalculateValue(const std::vector<KeyframeQuaternion>& keyframes, float time) {
	assert(!keyframes.empty());

	if (keyframes.size() == 1 || time <= keyframes[0].time) {
		return keyframes[0].value;
	}

	for (size_t index = 0; index < keyframes.size() - 1; ++index) {
		size_t nextIndex = index + 1;
		// indexとnextIndexの2つのKeyframeを取得して範囲内に時刻があるかを判定
		if (keyframes[index].time <= time && time <= keyframes[nextIndex].time) {
			// 範囲内を補完する
			float t = (time - keyframes[index].time) / (keyframes[nextIndex].time - keyframes[index].time);
			return Slerp(keyframes[index].value, keyframes[nextIndex].value, t);
		}
	}

	return (*keyframes.rbegin()).value;
}

std::array<float, 7> MAGIUtility::GenerateGaussianWeights(float sigma) {
	std::array<float, 7> weights{};
	float sum = 0.0f;

	for (int i = 0; i <= 3; ++i) {
		float x = static_cast<float>(i);
		weights[i] = std::exp(-(x * x) / (2.0f * sigma * sigma));
		sum += (i == 0) ? weights[i] : weights[i] * 2.0f; // 対称のため2倍
	}

	// 正規化
	for (float& w : weights) w /= sum;

	return weights;
}

uint32_t MAGIUtility::DivRoundUp(uint32_t num, uint32_t den) {
	return (num + den - 1) / den;
}