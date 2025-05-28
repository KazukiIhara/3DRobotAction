#pragma once

/// <summary>
/// シンプルアニメーションクラス
/// </summary>
/// <typeparam name="T"></typeparam>
template<typename T>
class SimpleAnimation {
public:
	enum class EasingType {
		Linear,
		EaseIn,
		EaseOut,
		EaseInOut
	};

	enum class LoopType {
		Restart,
		PingPong
	};

	SimpleAnimation(
		const T& start = T{},
		const T& end = T{},
		EasingType easing = EasingType::Linear,
		bool loop = false,
		LoopType loopType = LoopType::Restart
	);
	~SimpleAnimation();

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

	// イージング関数
	float Ease(float t) const {
		switch (easing_) {
		case EasingType::EaseIn:
			return t * t;
		case EasingType::EaseOut:
			return 1.0f - (1.0f - t) * (1.0f - t);
		case EasingType::EaseInOut:
			if (t < 0.5f) return 2.0f * t * t;
			return 1.0f - 2.0f * (1.0f - t) * (1.0f - t);
		case EasingType::Linear:
		default:
			return t;
		}
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

template<typename T>
inline SimpleAnimation<T>::SimpleAnimation(const T& start, const T& end, EasingType easing, bool loop, LoopType loopType) {

}

template<typename T>
inline SimpleAnimation<T>::~SimpleAnimation() {

}
