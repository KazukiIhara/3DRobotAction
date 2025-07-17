#pragma once


#include "Math/Utility/MathUtility.h"
#include "GameCommon/GameCommon.h"

/// <summary>
/// 攻撃コライダー
/// </summary>
class AttackCollider {
public:
	AttackCollider(const Vector3& posW, const Vector3& minL, const Vector3& maxL);
	~AttackCollider() = default;

	void Update();

	void Draw();

	void SetWorldPos(const Vector3& posW);

private:
	// ワールド上の座標
	Vector3 posW_;

	// ワールド上のminmax
	Vector3 minW_;
	Vector3 maxW_;

	// ローカルのminmax
	Vector3 minL_;
	Vector3 maxL_;
};