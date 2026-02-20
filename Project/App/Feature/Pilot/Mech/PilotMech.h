#pragma once

// 機体基底クラス
#include "Feature/Mech/Base/BaseMech.h"
// ステート基底クラス
#include "Feature/Pilot/Mech/State/IPilotMechState.h"
// ジャスト回避コライダー
#include "Feature/Pilot/Mech/JustDodgeCollider/PilotMechJustDodgeCollider.h"
// ロックオンシステム
#include "Feature/Pilot/Mech/System/LockOn/PilotMechLockOnSystem.h"
// 上下移動システム
#include "Feature/Pilot/Mech/System/VerticalMove/PilotMechVerticalMoveSystem.h"
// 機体のアクション管理システム
#include "Feature/Pilot/Mech/System/CombatAction/PilotMechCombatActionSystem.h"
// ステータスクラス
#include "Feature/Pilot/Mech/Status/PilotMechStatus.h"

// 前方宣言
class GameInputSystem;
class TPSCamera3D;

// 
// 以下改修予定
//
class BossMech;

/// <summary>
/// パイロット機体クラス
/// </summary>
class PilotMech :public BaseMech {
public:
	enum class State {
		Idle,
		Move,
		Dodge,
		JustDodge,
		JustDodgeAttack,
		HitReact,
		KnockBack
	};
public:
	PilotMech(const InitParam& param, const BaseMech::RefContext& ref, GameInputSystem* InputSys);
	~PilotMech() = default;

	// 更新
	void Update([[maybe_unused]] bool isShowDebugUI, const BaseMech::InitParam& param);
	// デバッグ描画
	void DebugDraw()override;
	// ステートを変更
	void ChangeState(PilotMech::State nextState);
	// 現在のステートを取得
	PilotMech::State GetCurrentState()const;
	// ジャスト回避コライダーを取得
	PilotMechJustDodgeCollider* GetJustDodgeCollider();
	// ロックオンシステムを取得
	PilotMechLockOnSystem* GetLockOnSystem();
	// 上下移動システムを取得
	PilotMechVerticalMoveSystem* GetVerticalMoveSystem();
	// アクション管理システムを取得
	PilotMechCombatActionSystem* GetCombatActionSystem();
	// ステータスクラスを取得s
	PilotMechStatus* GetStatus();

	// インプットシステムを取得
	GameInputSystem* GetInputSys();

	// 
	// 以下改修予定
	// 

	// ボス機体をロックオン対象にセット
	void SetBossMech(BossMech* mech);

private:
	// ステート取得
	IPilotMechState* GetState(PilotMech::State state);

	// ステート文字列変換
	const std::string StateToString(PilotMech::State state);

	// デバッグウィンドウ描画
	void ShowDebugWindow()override;

private:
	// ステートテーブル
	std::unordered_map<PilotMech::State, std::unique_ptr<IPilotMechState>>states_;

	// 現在のステート
	std::pair<PilotMech::State, IPilotMechState*> currentState_;

	// ジャスト回避コライダー
	std::unique_ptr<PilotMechJustDodgeCollider> justDodgeCollider_;

	// ロックオンシステム
	std::unique_ptr<PilotMechLockOnSystem> lockOnSystem_;

	// 上下移動システム
	std::unique_ptr<PilotMechVerticalMoveSystem> verticalMoveSystem_;

	// アクション管理システム
	std::unique_ptr<PilotMechCombatActionSystem> combatActSystem_;

	// ステータスクラス
	std::unique_ptr<PilotMechStatus> status_;

	// 入力システムの参照ポインタ
	GameInputSystem* inputSys_ = nullptr;

};
