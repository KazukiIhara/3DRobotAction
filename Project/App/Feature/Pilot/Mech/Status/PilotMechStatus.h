#pragma once

// C++
#include <cstdint>

#include "Math/Types/AllMathTypes.h"

// パイロット機体
class PilotMech;

/// <summary>
/// パイロット機体のステータス
/// </summary>
class PilotMechStatus {
public:
	struct Param {
		// 体力
		int32_t hp;
		// 落下死亡フラグ
		bool dropped;
		// 連続ジャスト回避回数
		int32_t justDodgeStreak;
		// ジャスト回避スコア	
		int32_t justDodgeScore;
		// 攻撃倍率
		float attackMul;
	};
public:
	PilotMechStatus(PilotMech* mech);
	~PilotMechStatus() = default;

	void Update();

	Vector3 GetHitPos()const;

	int32_t GetHP()const;
	int32_t GetMaxHP()const;

	bool GetIsDropped()const;

	int32_t GetJustDodgeStreak()const;

	int32_t GetJustDodgeScore()const;

	float GetAttackMul()const;

private:
	void ReactHitInfo();
	void JudgeDropped();
private:
	Vector3 hitPos_;
	Param param_{};
	float droppedHeight_ = 0.0f;
	int32_t maxHp_;
	PilotMech* mech_ = nullptr;
};