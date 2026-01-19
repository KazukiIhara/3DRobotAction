#include "PilotMech.h"

#include "MAGI.h"
#include "MAGIAssert/MAGIAssert.h"

#include "GameObject/Damage/Object/Manager/DamageObjectManager.h"
#include "GameEffects/System/GameEffectManager/GameEffectManager.h"
#include "MechAnimation/Container/MechAnimationContainer.h"

using namespace Magi;

PilotMech::PilotMech(const InitParam& param, const RefContext& ref) :
	BaseMech(param, ref) {

	// ステートテーブル作成


	// システム作成
	// 移動システム
	moveSystem_ = std::make_unique<PilotMechMoveSystem>(this);


	// 最初のステートを設定
	ChangeState(PilotMechState::Idle);

}

void PilotMech::Update([[maybe_unused]] bool isShowDebugUI, [[maybe_unused]] const BaseMech::InitParam& param) {
	// デバッグの更新
	DebugUpdate(isShowDebugUI, param);

	// ステート更新
	if (auto& state = currentState_.second) {
		state->Update(this);
	}

	// システム更新
	moveSystem_->Update();

	// 基底クラスの更新
	BaseMech::Update(isShowDebugUI, param);
}

void PilotMech::ChangeState(PilotMech::PilotMechState nextState) {

}

const std::string PilotMech::StateToString(PilotMech::PilotMechState state) {
	return std::string();
}

void PilotMech::ShowDebugWindow() {
	// デバッグ操作ウィンドウ
	ImGui::Begin("PlayerMech");
	ImGui::SeparatorText("Parameter");
	{
		// 現在ステート表示
		ImGui::Text("CurrentState :");
		ImGui::SameLine();
		const std::string state = StateToString(currentState_.first);
		ImGui::Text(state.c_str());
	}

	// デバッグフラグのUI
	ShowDebugFlagUI();

	ImGui::End();
}
