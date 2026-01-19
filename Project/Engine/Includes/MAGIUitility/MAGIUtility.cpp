#include "MAGIUtility.h"

#include "MAGI.h"

#include <cassert>

using namespace Magi;
using namespace MAGIMath;

Vector3 MAGIUtility::CalculateValue(const std::vector<KeyframeVector3>& keyframes, float time) {
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

Vector3 MAGIUtility::CalculateLoopValue(const std::vector<KeyframeVector3>& keyframes, float time) {
	assert(!keyframes.empty());

	// アニメーション全体の長さ
	const float duration = keyframes.back().time;

	// duration <= 0 のときは異常系。とりあえず先頭キーを返す
	if (duration <= 0.0f) {
		return keyframes.front().value;
	}

	// fmod で time を [0, duration) に巻き取る
	time = std::fmod(time, duration);
	if (time < 0.0f) {      // 負の値にも対応
		time += duration;
	}

	return CalculateValue(keyframes, time);
}

Quaternion MAGIUtility::CalculateLoopValue(const std::vector<KeyframeQuaternion>& keyframes, float time) {
	assert(!keyframes.empty());

	const float duration = keyframes.back().time;
	if (duration <= 0.0f) {
		return keyframes.front().value;
	}

	time = std::fmod(time, duration);
	if (time < 0.0f) {
		time += duration;
	}

	return CalculateValue(keyframes, time);
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

Vector4 MAGIUtility::NormalizePlane(const Vector4& plane) {
	float length = std::sqrt(plane.x * plane.x + plane.y * plane.y + plane.z * plane.z);
	return plane / length;
}

Vector2 MAGIUtility::TransformWorldToScreen(const Vector3& worldPos) {
	const Matrix4x4 vp = MAGISYSTEM::GetCurrentCamera3D()->GetViewProjectionMatrix();
	const Vector4 clip = Transform(Vector4(worldPos.x, worldPos.y, worldPos.z, 1.0f), vp);
	const float invW = 1.0f / clip.w;
	const Vector3 ndc{ clip.x * invW, clip.y * invW, clip.z * invW };
	const float sx = (ndc.x + 1.f) * 0.5f * WindowApp::kClientWidth;
	const float sy = (-ndc.y + 1.f) * 0.5f * WindowApp::kClientHeight;

	return Vector2(sx, sy);
}

Vector3 MAGIUtility::StickToMoveDirOnPlane(const Vector2& stick, const Vector3& cameraEye, const Vector3& cameraTarget, const Vector3& planeNormal) {
	// 平面法線を正規化
	const Vector3 up = SafeNormalize3(planeNormal);
	if (LengthSq3(up) <= 1.0e-8f) {
		return { 0.0f, 0.0f, 0.0f };
	}

	// カメラForwardを平面へ射影
	const Vector3 camFwd = cameraTarget - cameraEye;
	const Vector3 fwdOnPlane = SafeNormalize3(ProjectOnPlane(camFwd, up));
	if (LengthSq3(fwdOnPlane) <= 1.0e-8f) {
		return { 0.0f, 0.0f, 0.0f };
	}

	// Right生成
	const Vector3 rightOnPlane = SafeNormalize3(Cross(up, fwdOnPlane));
	if (LengthSq3(rightOnPlane) <= 1.0e-8f) {
		return { 0.0f, 0.0f, 0.0f };
	}

	// スティック→移動方向
	const Vector3 move = rightOnPlane * stick.x + fwdOnPlane * stick.y;
	return SafeNormalize3(move);
}

Vector2 MAGIUtility::StickToMoveDirXZ(const Vector2& stick, const Vector3& cameraEye, const Vector3& cameraTarget, const Vector3& planeNormal) {
	const Vector3 dir = StickToMoveDirOnPlane(stick, cameraEye, cameraTarget, planeNormal);

	// XZに落とす
	const Vector2 xz{ dir.x, dir.z };
	const float lsq = LengthSq2(xz);
	if (lsq <= 1.0e-8f) {
		return { 0.0f, 0.0f };
	}

	// 2D正規化
	const float len = std::sqrt(lsq);
	return { xz.x / len, xz.y / len };
}