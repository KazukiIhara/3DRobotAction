#include "PilotMech.h"

#include "MAGI.h"
#include "MAGIAssert/MAGIAssert.h"

#include "Feature/Damage/Object/Manager/DamageObjectManager.h"
#include "Feature/Effect/System/GameEffectManager/GameEffectManager.h"
#include "MechAnimation/Container/MechAnimationContainer.h"

// ステートクラス
#include "Feature/Pilot/Mech/State/Idle/PilotMechStateIdle.h"
#include "Feature/Pilot/Mech/State/Move/PilotMechStateMove.h"
#include "Feature/Pilot/Mech/State/Dodge/PilotMechStateDodge.h"
#include "Feature/Pilot/Mech/State/JustDodge/PilotMechStateJustDodge.h"

using namespace Magi;

PilotMech::PilotMech(const InitParam& param, const RefContext& ref, GameInputSystem* inputSys) :
	BaseMech(param, ref) {
	inputSys_ = inputSys;

	modelTransform_->SetScale(Vector3(0.5f, 0.5f, 0.5f));

	// 武器をマップに追加

	// システム作成
	// 移動システム
	moveSystem_ = std::make_unique<PilotMechMoveSystem>(this);
	modelDirSystem_ = std::make_unique<PilotMechModelDirSystem>(this);

	// ステートテーブル作成
	states_[State::Idle] = std::make_unique<PilotMechStateIdle>();
	states_[State::Move] = std::make_unique<PilotMechStateMove>();
	states_[State::Dodge] = std::make_unique<PilotMechStateDodge>();
	states_[State::JustDodge] = std::make_unique<PilotMechStateJustDodge>();

	// 最初のステートを設定
	ChangeState(State::Idle);

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
	modelDirSystem_->Update();

	// 基底クラスの更新
	BaseMech::Update(isShowDebugUI, param);
}

void PilotMech::ChangeState(PilotMech::State nextState) {
	// 旧ステートの終了処理
	if (auto cs = currentState_.second) {
		cs->Exit(this);
	}

	// 変更後ステートの開始処理
	currentState_ = std::make_pair(nextState, GetState(nextState));
	if (auto cs = currentState_.second) {
		cs->Enter(this);
	}
}

PilotMech::State PilotMech::GetCurrentState()const {
	return currentState_.first;
}

PilotMechMoveSystem* PilotMech::GetMoveSystem() {
	return moveSystem_.get();
}

GameInputSystem* PilotMech::GetInputSys() {
	return inputSys_;
}

IPilotMechState* PilotMech::GetState(PilotMech::State state) {
	// ステートテーブルから検索
	auto it = states_.find(state);
	if (it != states_.end()) {
		return it->second.get();
	}

	MAGIAssert::Assert(false, "Not find IPilotMechState!");
	return {};
}

const std::string PilotMech::StateToString(PilotMech::State state) {
	switch (state) {
		case PilotMech::State::Idle:
			return "Idle";
		case PilotMech::State::Move:
			return "Move";
		case PilotMech::State::Dodge:
			return "Dodge";
		case PilotMech::State::JustDodge:
			return "JustDodge";
		default:
			return "Unknown";
	}
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
