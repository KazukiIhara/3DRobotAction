#pragma once

// 数学ヘッダ
#include "Math/Utility/MathUtility.h"


#include "Structs/Primitive3DStruct.h"


namespace magi {
// 前方宣言
class Transform3D;

/// <summary>
/// 武器発砲時のマズルフラッシュ
/// </summary>
class MuzzleFlashEffect {
public:
	MuzzleFlashEffect();
	~MuzzleFlashEffect() = default;

	void Emit();
	void Update(const Vector3& worldPos);
	void Draw();

private:
	// 発火時間
	const float effectTime_ = 0.15f;
	float effectTimer_ = 0.0f;

	// ワールド行列
	Matrix4x4 wMat_;

	Vector3 scale_ = { 0.5f,0.5f,1.0f };

	// 板ポリデータ
	PlaneData3D data_;
	// 板ポリマテリアル
	MaterialData3D mat_;
};
}
