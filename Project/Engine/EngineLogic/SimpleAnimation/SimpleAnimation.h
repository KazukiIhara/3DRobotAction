#pragma once

// 数学ヘッダ
#include "Math/Utility/MathUtility.h"

using namespace MAGIMath;

enum class EasingType {
	Linear,

	// Sine
	EaseInSine, EaseOutSine, EaseInOutSine,
	// Quad（旧 EaseIn/EaseOut/EaseInOut と同じ）
	EaseInQuad, EaseOutQuad, EaseInOutQuad,
	// Cubic
	EaseInCubic, EaseOutCubic, EaseInOutCubic,
	// Quart
	EaseInQuart, EaseOutQuart, EaseInOutQuart,
	// Quint
	EaseInQuint, EaseOutQuint, EaseInOutQuint,
	// Expo
	EaseInExpo, EaseOutExpo, EaseInOutExpo,
	// Circ
	EaseInCirc, EaseOutCirc, EaseInOutCirc,
	// Back
	EaseInBack, EaseOutBack, EaseInOutBack,
	// Elastic
	EaseInElastic, EaseOutElastic, EaseInOutElastic,
	// Bounce
	EaseInBounce, EaseOutBounce, EaseInOutBounce,
};

enum class LoopType {
	Restart,
	PingPong
};

/// <summary>
/// シンプルアニメーションクラス
/// </summary>
/// <typeparam name="T"></typeparam>
template<typename T>
class SimpleAnimation {
public:

	SimpleAnimation(
		const T& start = T{},
		const T& end = T{},
		EasingType easing = EasingType::Linear,
		bool loop = false,
		LoopType loopType = LoopType::Restart
	) {
		startValue_ = start;
		endValue_ = end;
		easing_ = easing;
		loop_ = loop;
		loopType_ = loopType;
	}

	T GetValue(float t) const {
		float u = NormalizeTime(t);
		float e = Ease(u);
		return Lerp(startValue_, endValue_, e);
	}

	// セッター
	void SetStart(const T& v) {
		startValue_ = v;
	}
	void SetEnd(const T& v) {
		endValue_ = v;
	}
	void SetEasing(EasingType e) {
		easing_ = e;
	}
	void SetLoop(bool on) {
		loop_ = on;
	}
	void SetLoopType(LoopType loopType) {
		loopType_ = loopType;
	}

private:
	// t → [0,1] もしくはループ処理後の u を返す
	float NormalizeTime(float t) const {
		if (!loop_) {
			// ループなしは 0–1 にクランプ
			return std::clamp(t, 0.0f, 1.0f);
		}
		// ループあり
		switch (loopType_) {
		case LoopType::Restart:
			// 1.0 ごとにリセット
			return t - std::floor(t);
		case LoopType::PingPong: {
			// 0–2 のサイクルを折り返し
			float cycle = t - std::floor(t / 2.0f) * 2.0f;
			if (cycle <= 1.0f) return cycle;
			return 2.0f - cycle;
		}
		default:
			return std::clamp(t, 0.0f, 1.0f);
		}
	}

	// 0.0f〜1.0f 以外を入れる場合は外側で正規化してください
	float Ease(float t) const {
		constexpr float PI = 3.14159265358979323846f;
		constexpr float c1 = 1.70158f;
		constexpr float c2 = c1 * 1.525f;
		constexpr float n1 = 7.5625f;
		constexpr float d1 = 2.75f;

		auto bounceOut = [&](float x) {
			if (x < 1.0f / d1)
				return n1 * x * x;
			if (x < 2.0f / d1) {
				x -= 1.5f / d1;
				return n1 * x * x + 0.75f;
			}
			if (x < 2.5f / d1) {
				x -= 2.25f / d1;
				return n1 * x * x + 0.9375f;
			}
			x -= 2.625f / d1;
			return n1 * x * x + 0.984375f;
			};

		switch (easing_) {
			// Linear
		case EasingType::Linear:
			return t;

			// Sine
		case EasingType::EaseInSine:
			return 1.0f - std::cos((t * PI) / 2.0f);
		case EasingType::EaseOutSine:
			return std::sin((t * PI) / 2.0f);
		case EasingType::EaseInOutSine:
			return -(std::cos(PI * t) - 1.0f) * 0.5f;

			// Quad
		case EasingType::EaseInQuad:
			return t * t;
		case EasingType::EaseOutQuad:
			return 1.0f - (1.0f - t) * (1.0f - t);
		case EasingType::EaseInOutQuad:
			return (t < 0.5f)
				? 2.0f * t * t
				: 1.0f - std::pow(-2.0f * t + 2.0f, 2.0f) * 0.5f;

			// Cubic
		case EasingType::EaseInCubic:
			return t * t * t;
		case EasingType::EaseOutCubic:
			return 1.0f - std::pow(1.0f - t, 3.0f);
		case EasingType::EaseInOutCubic:
			return (t < 0.5f)
				? 4.0f * t * t * t
				: 1.0f - std::pow(-2.0f * t + 2.0f, 3.0f) * 0.5f;

			// Quart
		case EasingType::EaseInQuart:
			return t * t * t * t;
		case EasingType::EaseOutQuart:
			return 1.0f - std::pow(1.0f - t, 4.0f);
		case EasingType::EaseInOutQuart:
			return (t < 0.5f)
				? 8.0f * t * t * t * t
				: 1.0f - std::pow(-2.0f * t + 2.0f, 4.0f) * 0.5f;

			// Quint
		case EasingType::EaseInQuint:
			return t * t * t * t * t;
		case EasingType::EaseOutQuint:
			return 1.0f - std::pow(1.0f - t, 5.0f);
		case EasingType::EaseInOutQuint:
			return (t < 0.5f)
				? 16.0f * t * t * t * t * t
				: 1.0f - std::pow(-2.0f * t + 2.0f, 5.0f) * 0.5f;

			// Expo
		case EasingType::EaseInExpo:
			return (t == 0.0f) ? 0.0f : std::pow(2.0f, 10.0f * t - 10.0f);
		case EasingType::EaseOutExpo:
			return (t == 1.0f) ? 1.0f : 1.0f - std::pow(2.0f, -10.0f * t);
		case EasingType::EaseInOutExpo:
			if (t == 0.0f) return 0.0f;
			if (t == 1.0f) return 1.0f;
			return (t < 0.5f)
				? 0.5f * std::pow(2.0f, 20.0f * t - 10.0f)
				: 0.5f * (2.0f - std::pow(2.0f, -20.0f * t + 10.0f));

			// Circ
		case EasingType::EaseInCirc:
			return 1.0f - std::sqrt(1.0f - t * t);
		case EasingType::EaseOutCirc:
			return std::sqrt(1.0f - std::pow(t - 1.0f, 2.0f));
		case EasingType::EaseInOutCirc:
			return (t < 0.5f)
				? (1.0f - std::sqrt(1.0f - 4.0f * t * t)) * 0.5f
				: (std::sqrt(1.0f - std::pow(-2.0f * t + 2.0f, 2.0f)) + 1.0f) * 0.5f;

			// Back
		case EasingType::EaseInBack:
			return (c1 + 1.0f) * t * t * t - c1 * t * t;
		case EasingType::EaseOutBack:
			return 1.0f + (c1 + 1.0f) * std::pow(t - 1.0f, 3.0f) + c1 * std::pow(t - 1.0f, 2.0f);
		case EasingType::EaseInOutBack:
			return (t < 0.5f)
				? std::pow(2.0f * t, 2.0f) * ((c2 + 1.0f) * 2.0f * t - c2) * 0.5f
				: (std::pow(2.0f * t - 2.0f, 2.0f) * ((c2 + 1.0f) * (2.0f * t - 2.0f) + c2) + 2.0f) * 0.5f;

			// Elastic
		case EasingType::EaseInElastic: {
			const float c4 = (2.0f * PI) / 3.0f;
			if (t == 0.0f) return 0.0f;
			if (t == 1.0f) return 1.0f;
			return -std::pow(2.0f, 10.0f * t - 10.0f) * std::sin((t * 10.0f - 10.75f) * c4);
		}
		case EasingType::EaseOutElastic: {
			const float c4 = (2.0f * PI) / 3.0f;
			if (t == 0.0f) return 0.0f;
			if (t == 1.0f) return 1.0f;
			return std::pow(2.0f, -10.0f * t) * std::sin((t * 10.0f - 0.75f) * c4) + 1.0f;
		}
		case EasingType::EaseInOutElastic: {
			const float c5 = (2.0f * PI) / 4.5f;
			if (t == 0.0f) return 0.0f;
			if (t == 1.0f) return 1.0f;
			return (t < 0.5f)
				? -0.5f * std::pow(2.0f, 20.0f * t - 10.0f) * std::sin((20.0f * t - 11.125f) * c5)
				: 0.5f * std::pow(2.0f, -20.0f * t + 10.0f) * std::sin((20.0f * t - 11.125f) * c5) + 1.0f;
		}


										 // Bounce							
		case EasingType::EaseOutBounce:
			return bounceOut(t);
		case EasingType::EaseInBounce:
			return 1.0f - bounceOut(1.0f - t);
		case EasingType::EaseInOutBounce:
			return (t < 0.5f)
				? 0.5f * (1.0f - bounceOut(1.0f - 2.0f * t))
				: 0.5f * bounceOut(2.0f * t - 1.0f) + 0.5f;
		}
		return t;
	}

private:
	// 開始の値
	T startValue_;
	// 終了の値
	T endValue_;
	// イージングのタイプ
	EasingType easing_;
	// ループするかどうか
	bool loop_;
	// ループの種類
	LoopType loopType_;

};