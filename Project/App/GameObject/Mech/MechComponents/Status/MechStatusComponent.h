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
	int32_t GetHp()const;
	int32_t GetMaxHp()const;
	float GetHPRaito()const;

	// ENを取得
	int32_t GetEn()const;
	int32_t GetMaxEn()const;
	float GetEnRaito()const;

	// オーバーヒート状態かどうかを取得
	const bool& GetIsOverheat()const;

	// 上昇時のエネルギー消費処理
	void UseUpBoostEnergy();
	// クイックブーストのエネルギー消費処理
	void UseQuickBoostEnergy();

	// FCS回避係数を取得
	float GetFcsAvoidFactor()const;

	// 硬直時間を取得
	float GetRecoveryTime()const;
	
	// 硬直時間をセット
	void SetRecoveryTime(float t);

private:
	// エネルギー消費処理
	void UseEnergy(const int32_t& enValue);
	// 被ダメージ処理
	void GetDamage(const int32_t& damage, MechCore* mechcore);
	// FCS解除
	void UpdateFcsAvoidFactor(MechCore* mechCore);
	// 硬直時間更新
	void UpdateRecoveryTime();

	// EN更新処理
	void ENUpdate();
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

	// EN回復クールタイム
	float enRecoveryCoolTimer_ = 0.0f;

	// オーバーヒートしているかどうか
	bool isOverHeat_ = false;
	// オーバーヒートタイマー
	float overHeatTimer_ = 0.0f;

	// FCS回避係数
	float fcsAvoidFactor_ = 0.0f;

	// 硬直時間
	float recoveryTime_ = 0.0f;

	// 
	// パラメータ
	// 

	// 最大HP
	const int32_t kMaxHP_ = 10000;

	// 最大EN
	const int32_t kMaxEN_ = 5000;
	// EN回復クールタイム
	const float kEnRecoveryCoolTime_ = 0.6f;
	// 秒間EN回復量
	const int32_t kEnRecoveryPerSec_ = 1500;

	// クイックブースト時消費EN量
	const int32_t kQuickBoostUseEn_ = 800;
	// 上昇ブースト時秒間消費EN
	const int32_t kUpBoostUseEnPerSec_ = 1200;
	// アサルトブースト時秒間消費EN
	const int32_t kAssultBoostUseEnPerSec_ = 9000;

	// FCSの復帰時間
	const float fcsRecoverTime_ = 0.3f;


	// 
	// 定数
	// 

	// 相手が回避状態の際のFCS強度
	const float minFcsFactor_ = 0.05f;
};