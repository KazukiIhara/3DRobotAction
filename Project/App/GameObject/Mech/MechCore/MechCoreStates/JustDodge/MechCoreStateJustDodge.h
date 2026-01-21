#pragma once


#include "GameObject/Mech/MechCore/MechCoreStates/MechCoreBaseState.h"

// 実装メモ
/*
	クイックブーストしている方向にさらに一瞬加速して一回転する 
	一回転する処理は胴体パーツの更新処理に実装する
*/

/// <summary>
/// ジャスト回避ステート
/// </summary>
class MechCoreStateJustDodge:public MechCoreBaseState {
public:
	MechCoreStateJustDodge();
	~MechCoreStateJustDodge() = default;
	void Enter(MechCore* mechCore)override;
	void Update(MechCore* mechCore)override;
	void Exit(MechCore* mechCore)override;
private:
	// ステート計測用タイマー
	float timer_ = 0.0f;
};