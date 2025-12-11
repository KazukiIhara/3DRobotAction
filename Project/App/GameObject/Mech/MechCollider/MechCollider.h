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
		magi::Vector3 attackPos;
		AttackType type;
		int32_t damage;
	};

	MechCollider(const FriendlyTag& tag, const magi::Vector3& posW, const magi::Vector3& minL, const magi::Vector3& maxL);
	~MechCollider() = default;

	void Update();

	void Draw();

	const magi::Vector3& GetMinW()const;
	const magi::Vector3& GetMaxW()const;

	void AddHitInfo(const HitInfo& info);

	bool GetIsAlive()const;
	std::vector<MechCollider::HitInfo> GetHitInfo()const;

	void SetWorldPos(const magi::Vector3& posW);
	void SetIsAlive(bool isAlve);

private:
	// 識別タグ
	FriendlyTag tag_;

	// 衝突インフォ
	std::vector<HitInfo> hitInfos_;

	// ワールド上の座標
	magi::Vector3 posW_;

	// ワールド上のminmax
	magi::Vector3 minW_;
	magi::Vector3 maxW_;

	// ローカルのminmax
	magi::Vector3 minL_;
	magi::Vector3 maxL_;

	// 生存フラグ
	bool isAlive_ = true;

};