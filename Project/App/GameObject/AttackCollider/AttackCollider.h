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

	bool GetIsAlive()const;

	void SetWorldPos(const Vector3& posW);
	void SetIsAlive(bool isAlve);

private:
	// ワールド上の座標
	Vector3 posW_;

	// ワールド上のminmax
	Vector3 minW_;
	Vector3 maxW_;

	// ローカルのminmax
	Vector3 minL_;
	Vector3 maxL_;

	// 生存フラグ
	bool isAlive_ = true;
};