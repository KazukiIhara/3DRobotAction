#pragma once

// C++
#include <unordered_map>
#include <string>

// DirectX
#include <d3d12.h>

// MyHedder
#include "Math/Utility/MathUtility.h"

// Vector3のアニメーションキーフレーム
struct KeyframeVector3 {
	Vector3 value;
	float time;
};

// Quaternionのアニメーションキーフレーム
struct KeyframeQuaternion {
	Quaternion value;
	float time;
};

// Nodeアニメーション
struct NodeAnimation {
	std::vector<KeyframeVector3> translate;
	std::vector<KeyframeQuaternion> rotate;
	std::vector<KeyframeVector3> scale;
};

// アニメーション
struct AnimationData {
	std::string name;
	float duration;
	std::unordered_map<std::string, NodeAnimation> nodeAnimations;
};