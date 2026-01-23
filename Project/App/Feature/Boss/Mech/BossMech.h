#pragma once

// 機体基底クラス
#include "Feature/Mech/Base/BaseMech.h"

// ステート基底クラス
#include "Feature/Boss/Mech/State/BossMechBaseState.h"

// バリア
#include "Feature/Boss/Mech/Barirer/BossMechBarrier.h"

// ステータスクラス
#include "Feature/Boss/Mech/Status/BossMechStatus.h"

// プレイヤー機体
class PilotMech;

/// <summary>
/// ボス機体クラス
/// </summary>
class BossMech:public BaseMech {
public:
	// ステート
	enum class State {
		Idle,
		LaserShot
	};
public:
	BossMech(const InitParam& param, const RefContext& ref, PilotMech* pilotMech);
	~BossMech() = default;

	void Update([[maybe_unused]] bool isShowDebugUI, const BaseMech::InitParam& param)override;
	// ステート切り替え
	void ChangeState(BossMech::State nextState);

	// バリアを取得
	BossMechBarrier* GetBarrier();

	// ステータスを取得
	BossMechStatus* GetStatus();

	// パイロット機体の参照を取得
	PilotMech* GetPilotMech();

private:
	// 対応するステートを取得
	BossMechBaseState* GetState(BossMech::State state);
	// ステートを文字列に変換
	const std::string StateToString(BossMech::State state);

	// デバッグウィンドウ描画
	void ShowDebugWindow()override;

private:
	// ステートテーブル
	std::unordered_map<BossMech::State, std::unique_ptr<BossMechBaseState>> states_;

	// 現在のステート
	std::pair<BossMech::State, BossMechBaseState*> currentState_;

	// バリア
	std::unique_ptr<BossMechBarrier> barrier_;

	// ステータス
	std::unique_ptr<BossMechStatus> status_;

	// プレイヤー機体の参照ポインタ
	PilotMech* pilotMech_;

};