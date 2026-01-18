#pragma once

// C++
#include <array>
#include <string>
#include <vector>

// 数学
#include "Math/Utility/MathUtility.h"

#include "GameObject/Boss/Mech/BossMech.h"

namespace MechAnimation {
	static constexpr size_t kJointCount = static_cast<size_t>(BossMech::TransType::Count);

	/// <summary>
	/// 1フレーム分の関節回転（FK）
	/// </summary>
	struct Pose {
		std::array<Quaternion, kJointCount> rotations{};
	};

	/// <summary>
	/// 時間情報を持たないFKアニメーションクリップ
	/// 呼び出し側で 0.0f～1.0f を制御して再生する
	/// </summary>
	struct Clip {
		std::vector<Pose> frames;
	};

	/// <summary>
	/// 0.0f～1.0f をクランプ
	/// </summary>
	inline float Clamp01(float t) {
		if (t < 0.0f) { return 0.0f; }
		if (t > 1.0f) { return 1.0f; }
		return t;
	}

	/// <summary>
	/// フレームが0/1のときも安全に扱うための取得
	/// </summary>
	inline const Pose* GetPose(const Clip& clip, size_t frameIndex) {
		if (clip.frames.empty()) { return nullptr; }
		if (frameIndex >= clip.frames.size()) { return &clip.frames.back(); }
		return &clip.frames[frameIndex];
	}

	/// <summary>
	/// 正規化時間 t(0..1) からフレーム位置を取得（補間用）
	/// 返り値: i0, i1, alpha（i0->i1 の補間係数）
	/// </summary>
	inline void GetFrameLerpInfo(const Clip& clip, float t, size_t& outI0, size_t& outI1, float& outAlpha) {
		outI0 = 0;
		outI1 = 0;
		outAlpha = 0.0f;

		const size_t count = clip.frames.size();
		if (count == 0) { return; }
		if (count == 1) { return; }

		t = Clamp01(t);

		const float f = t * static_cast<float>(count - 1);
		const size_t i0 = static_cast<size_t>(f);
		const size_t i1 = (i0 + 1 < count) ? (i0 + 1) : i0;
		const float a = f - static_cast<float>(i0);

		outI0 = i0;
		outI1 = i1;
		outAlpha = a;
	}
}

