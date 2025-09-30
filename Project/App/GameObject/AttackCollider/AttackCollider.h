#pragma once


#include "Math/Utility/MathUtility.h"
#include "GameCommon/GameCommon.h"

/// <summary>
/// 攻撃コライダー
/// </summary>
class AttackCollider {
public:
	// 衝突に関するデータ
	struct HitInfo {
		bool isHit_ = false;
	};

	AttackCollider(
		const FriendlyTag& tag,
		const AttackType& type,
		const Vector3& posW,
		const Vector3& minL,
		const Vector3& maxL,
		int32_t damage);

	~AttackCollider() = default;

	void Update();

	void Draw();

	const Vector3& GetMinW()const;
	const Vector3& GetMaxW()const;

	bool GetIsAlive()const;
	HitInfo GetHitInfo()const;
	AttackType GetType()const;
	int32_t GetDamage()const;
	FriendlyTag GetFriendlyTag()const;

	void SetWorldPos(const Vector3& posW);
	void SetIsAlive(bool isAlve);
	void SetHitInfo(HitInfo hitInfo);

private:
	// 識別タグ
	FriendlyTag tag_;

	// この攻撃の種類
	AttackType type_;

	// 衝突インフォ
	HitInfo hitInfo_;

	// ワールド上の座標
	Vector3 posW_;

	// ワールド上のminmax
	Vector3 minW_;
	Vector3 maxW_;

	// ローカルのminmax
	Vector3 minL_;
	Vector3 maxL_;

	// ダメージ
	int32_t damage_ = 0;

	// 生存フラグ
	bool isAlive_ = true;
};