#pragma once

// 数学ヘッダ
#include "Math/Utility/MathUtility.h"

#include "Easing/Easing.h"

using namespace MAGIMath;

/// <summary>
/// ループタイプ
/// </summary>
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

	// 
	// ゲッター
	// 

	T GetValue(float t) const {
		float u = NormalizeTime(t);
		float easedT = Easing::Apply(easing_, u);
		return Lerp(startValue_, endValue_, easedT);
	}

	T GetStartValue() const {
		return startValue_;
	}

	T GetEndValue() const {
		return endValue_;
	}

	// 
	// セッター
	// 

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