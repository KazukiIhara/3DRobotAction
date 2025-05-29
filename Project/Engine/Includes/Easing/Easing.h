#pragma once

/// <summary>
/// イージングのタイプ
/// </summary>
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

/// <summary>
/// イージングまとめ
/// </summary>
namespace Easing {

	// enumで呼び出せる関数
	float Apply(EasingType type, float t);

	// Linear
	float EaseLinear(float t);

	// Sine
	float EaseInSine(float t);
	float EaseOutSine(float t);
	float EaseInOutSine(float t);

	// Quad
	float EaseInQuad(float t);
	float EaseOutQuad(float t);
	float EaseInOutQuad(float t);

	// Cubic
	float EaseInCubic(float t);
	float EaseOutCubic(float t);
	float EaseInOutCubic(float t);

	// Quart
	float EaseInQuart(float t);
	float EaseOutQuart(float t);
	float EaseInOutQuart(float t);

	// Quint
	float EaseInQuint(float t);
	float EaseOutQuint(float t);
	float EaseInOutQuint(float t);

	// Expo
	float EaseInExpo(float t);
	float EaseOutExpo(float t);
	float EaseInOutExpo(float t);

	// Circ
	float EaseInCirc(float t);
	float EaseOutCirc(float t);
	float EaseInOutCirc(float t);

	// Back
	float EaseInBack(float t);
	float EaseOutBack(float t);
	float EaseInOutBack(float t);

	// Elastic
	float EaseInElastic(float t);
	float EaseOutElastic(float t);
	float EaseInOutElastic(float t);

	// Bounce
	float EaseInBounce(float t);
	float EaseOutBounce(float t);
	float EaseInOutBounce(float t);
}
