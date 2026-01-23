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


#include "MAGIAssert/MAGIAssert.h"

using namespace CombatSceneControl;

CombatSceneController::CombatSceneController(ContextRef ref) {
	// 参照ポインタ受け取り
	ref_ = ref;

	// ステートテーブルを作成


	// 最初のステートを設定


}

void CombatSceneController::Update() {
	// ステート更新
	if (auto& state = currentState_.second) {
		state->Update(ref_);
	}
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

ICombatSceneState* CombatSceneController::GetState(CombatSceneController::State state) {
	// ステートテーブルから検索
	auto it = states_.find(state);
	if (it != states_.end()) {
		return it->second.get();
	}

	MAGIAssert::Assert(false, "Not find CombatSceneState!");
	return {};
}