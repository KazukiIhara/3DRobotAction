#pragma once

// C++
#include <array>
#include <memory>

// MyHedder
#include "3D/Transform3D/Transform3D.h"
#include "Structs/Primitive3DStruct.h"
#include "Math/Types/AllMathTypes.h"
#include "SimpleAnimation/SimpleAnimation.h"

// エフェクトの最大数
static const uint32_t kMaxEffectNum = 65536;

/// <summary>
/// エフェクトのフェーズ
/// </summary>
enum class EffectPhase {
	Phase0,
	Phase1,
	Phase2,
};

/// <summary>
/// エフェクトアニメーション構造体
/// </summary>
template<typename T>
struct EffectAnimation {
	// アニメーションさせるかどうか
	bool isAnimated = false;

	// 何秒目からアニメーションを始めるか
	float startTime = 0.0f;
	// アニメーション時間
	float totalTime = 1.0f;

	// アニメーション
	SimpleAnimation<T> animation;

	// Tの値を求める
	T GetCurrentValue(float currentTime) {
		// アニメーションしないため動かさない
		if (!isAnimated) {
			return 0.0f;
		}
		// まだ開始前
		if (currentTime < startTime) {
			return 0.0f;
		}

		// アニメーション区間内
		const float local = currentTime - startTime;

		// 値を取得
		return animation.GetValue(local / totalTime);
	}
};

/// <summary>
/// 板ポリエフェクトパラメータ構造体
/// </summary>
struct PlaneEffectParam {

	// 
	// トランスフォーム
	// 

	EffectAnimation<Vector3> scale;
	EffectAnimation<Vector3> rotate;
	EffectAnimation<Vector3> translate;

	// 
	// マテリアル
	// 

	EffectAnimation<Vector2> uvScale;
	EffectAnimation<float> uvRotate;
	EffectAnimation<Vector2> uvTranslate;
	EffectAnimation<Vector4> color;
	std::string textureName;
	BlendMode blendMode = BlendMode::None;

	// 
	// 形状
	// 

	EffectAnimation<PlaneData3D> shape;

	// アニメーション時間
	float totalTime = 1.0f;
};

/// <summary>
/// 板ポリエフェクトデータ構造体
/// </summary>
struct PlaneEffect {
	PlaneEffectParam param;						// エフェクトのパラメータ
	Transform3D* currentTrans;					// 現在のトランスフォーム
	MaterialData3D currentMaterial;				// 現在のマテリアル
	PlaneData3D currentShape;					// 現在の形状データ
	float currentTime = 0.0f;					// アニメーションの経過時間
	bool isActive = true;						// 有効フラグ
};
