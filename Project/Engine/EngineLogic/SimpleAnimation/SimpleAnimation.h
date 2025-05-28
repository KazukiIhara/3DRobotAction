#pragma once

/// <summary>
/// シンプルアニメーションクラス
/// </summary>
/// <typeparam name="T"></typeparam>
template<typename T>
class SimpleAnimation {
public:
	enum class EasingType { Linear, EaseIn, EaseOut, EaseInOut };
	enum class LoopType { Restart, PingPong };

	SimpleAnimation(
		const T& start = T{},
		const T& end = T{},
		EasingType easing = EasingType::Linear,
		bool loop = false,
		LoopType loopType = LoopType::Restart
	);
	~SimpleAnimation();

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
