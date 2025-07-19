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

	void Update(MechCore* mechCore);

	// HPを取得
	const int32_t& GetHp()const;

private:
	// 機体の耐久力
	int32_t hp_ = 10000;

	// 
	// パラメータ
	// 

	// 最大HP
	const int32_t kMaxHP_ = 10000;
};