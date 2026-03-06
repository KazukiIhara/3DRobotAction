#pragma once


#include "Feature/Effect/System/BaseGameEffect/BaseGameEffect.h"
#include "Structs/Primitive3DStruct.h"

// 前方宣言
class Transform3D;

/// <summary>
/// マシンガンのヒットエフェクト
/// </summary>
class MachineGunHitEffect :public BaseGameEffect {
public:
	MachineGunHitEffect(const Vector3& emitPos);
	~MachineGunHitEffect() = default;

	void Update()override;
	void Draw()override;

private:
	// エフェクトタイマー
	float time_ = 0.0f;

	// リングのエフェクト用変数
	Vector3	ringTrans_;
	RingData3D ringData_;
	MaterialData3D ringMat_;


};