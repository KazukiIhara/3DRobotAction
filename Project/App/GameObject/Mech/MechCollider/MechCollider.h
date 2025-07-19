#pragma once

#include <vector>

#include "Math/Utility/MathUtility.h"
#include "GameCommon/GameCommon.h"

/// <summary>
/// 機体のコライダー
/// </summary>
class MechCollider {
public:
	// 衝突に関するデータ
	struct HitInfo {
		bool isHit = false;
		AttackType type;
		int32_t damage;
	};

	MechCollider(const FriendlyTag& tag, const Vector3& posW, const Vector3& minL, const Vector3& maxL);
	~MechCollider() = default;

	void Update();

	void Draw();

	const Vector3& GetMinW()const;
	const Vector3& GetMaxW()const;

	void AddHitInfo(const HitInfo& info);

	bool GetIsAlive()const;
	std::vector<MechCollider::HitInfo> GetHitInfo()const;

	void SetWorldPos(const Vector3& posW);
	void SetIsAlive(bool isAlve);

private:
	// 識別タグ
	FriendlyTag tag_;

	// 衝突インフォ
	std::vector<HitInfo> hitInfos_;

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