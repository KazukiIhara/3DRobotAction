#include "CombatSceneController.h"

//-------------------------------------------
// 操作クラス
//-------------------------------------------
#include "Feature/GameInputSystem/GameInputSystem.h"

// カメラ
#include "Feature/TPSCamera3D/TPSCamera3D.h"

// パイロット
#include "Feature/Pilot/Pilot.h"

// ボス
#include "Feature/Boss/Boss.h"

//-------------------------------------------
// マネージャ
//-------------------------------------------
#include "Feature/Effect/System/GameEffectManager/GameEffectManager.h"
#include "Feature/Damage/Object/Manager/DamageObjectManager.h"
#include "Feature/Damage/CollisionSystem/DamageCollisionSystem.h"


// シーンステート
#include "Feature/SceneStateController/CombatSceneController/States/Start/CombatSceneStateStart.h"
#include "Feature/SceneStateController/CombatSceneController/States/Battle/CombatSceneStateBattle.h"
#include "Feature/SceneStateController/CombatSceneController/States/End/CombatSceneStateEnd.h"
#include "Feature/SceneStateController/CombatSceneController/States/Pause/CombatSceneStatePause.h"

#include "MAGIAssert/MAGIAssert.h"

using namespace CombatSceneControl;

CombatSceneController::CombatSceneController(ContextRef ref) {
	// 参照ポインタ受け取り
	ref_ = {
		ref.inputSys,
		ref.camera,
		ref.pilot,
		ref.boss,
		ref.effectMgr,
		ref.collisionSys,
		ref.damageObjMgr,
		this
	};

	// ステートテーブルを作成
	states_[State::None] = nullptr;

	states_[State::Start] = std::make_unique<CombatSceneStateStart>();
	states_[State::Battle] = std::make_unique<CombatSceneStateBattle>();
	states_[State::End] = std::make_unique<CombatSceneStateEnd>();
	states_[State::Pause] = std::make_unique<CombatSceneStatePause>();

	// 文字列取得用マップを作成
	statesStrMap_[State::None] = "None";
	statesStrMap_[State::Start] = "Start";
	statesStrMap_[State::Battle] = "Battle";
	statesStrMap_[State::End] = "End";
	statesStrMap_[State::Pause] = "Pause";

	// 初期状態　何もしない
	currentState_ = std::make_pair(CombatSceneController::State::None, GetState(CombatSceneController::State::None));

	// 終了通知フラグ
	isEnd_ = false;
}

void CombatSceneController::Start(CombatSceneController::State state) {
	// ステート開始
	ChangeState(state);

	// 開始
	isEnd_ = false;
}

void CombatSceneController::End() {
	// ステートの終了処理
	if (auto cs = currentState_.second) {
		cs->Exit(ref_);
	}
	currentState_ = std::make_pair(CombatSceneController::State::None, GetState(CombatSceneController::State::None));

	// 終了
	isEnd_ = true;
}

bool CombatSceneController::Update() {
	// ステート更新
	if (auto& state = currentState_.second) {
		state->Update(ref_);
	}
	return isEnd_;
}

void CombatSceneController::Draw() {
	// ステート描画
	if (auto& state = currentState_.second) {
		state->Draw(ref_);
	}
}

void CombatSceneController::ChangeState(CombatSceneController::State state) {
	// 旧ステートの終了処理
	if (auto cs = currentState_.second) {
		cs->Exit(ref_);
	}

	// 変更後ステートの開始処理
	currentState_ = std::make_pair(state, GetState(state));
	if (auto cs = currentState_.second) {
		cs->Enter(ref_);
	}
}

std::string CombatSceneController::GetCurrentStateStr() {
	return GetStateStr(currentState_.first);
}

ICombatSceneState* CombatSceneController::GetState(CombatSceneController::State state) {
	// ステートテーブルから検索
	auto it = states_.find(state);
	if (it != states_.end()) {
		return it->second.get();
	}

	MAGIAssert::Assert(false, "Not find CombatSceneState!");
	return {};
}

std::string CombatSceneController::GetStateStr(CombatSceneController::State state) {
	// ステートテーブルから検索
	auto it = statesStrMap_.find(state);
	if (it != statesStrMap_.end()) {
		return it->second;
	}

	return "UNKNOWN";
}
