#pragma once

// MyHedder
#include "Math/Utility/MathUtility.h"
#include "SimpleAnimation/SimpleAnimation.h"

// 前方宣言
class MechCore;

/// <summary>
/// 機体のステータス管理コンポーネント
/// </summary>
class MechStatusComponent {
public:
	MechStatusComponent();
	~MechStatusComponent() = default;

	// 更新
	void Update(MechCore* mechCore);

	// HPを取得
	const int32_t& GetHp()const;
	const int32_t& GetMaxHp()const;
	float GetHPRaito()const;

	// ENを取得
	const int32_t& GetEn()const;
	const int32_t& GetMaxEn()const;
	float GetEnRaito()const;

	// オーバーヒート状態かどうかを取得
	const bool& GetIsOverheat()const;

	// クイックブーストのエネルギー消費処理
	void UseQuickBoostEnergy();
private:
	// エネルギー消費処理
	void UseEnergy(const int32_t& enValue);
private:
	// 
	// 機体のステータス変数
	// 

	// 機体の耐久力
	int32_t hp_ = 0;
	// 機体の耐久力の割合
	float hpRaito_ = 0.0f;

	// EN
	int32_t en_ = 0;
	// ENの割合
	float enRaito_ = 0.0f;

	// オーバーヒートしているかどうか
	bool isOverHeat_ = false;
	// オーバーヒートタイマー
	float overHeatTimer_ = 0.0f;

	// 
	// パラメータ
	// 

	// 最大HP
	const int32_t kMaxHP_ = 10000;

	// 最大EN
	const int32_t kMaxEN_ = 5000;
	// 秒間EN回復量
	const int32_t kEnRecoveryPerSec_ = 2000;

	// クイックブースト時消費EN量
	const int32_t kQuickBoostUseEn_ = 800;
	// アサルトブースト時秒間消費EN
	const int32_t kAssultBoostUseEnPerSec_ = 9000;

};