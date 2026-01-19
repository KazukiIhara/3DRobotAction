#include "BossMech.h"

#include "MAGI.h"
#include "MAGIAssert/MAGIAssert.h"

#include "GameObject/Damage/Object/Manager/DamageObjectManager.h"
#include "GameEffects/System/GameEffectManager/GameEffectManager.h"
#include "MechAnimation/Container/MechAnimationContainer.h"

// ステートクラス
#include "Feature/Boss/Mech/State/Idle/BossMechStateIdle.h"
#include "Feature/Boss/Mech/State/LaserShot/BossMechStateLaserShot.h"

using namespace Magi;

BossMech::BossMech(const InitParam& param, const RefContext& ref) :
	BaseMech(param, ref) {

	// 武器をマップに追加
	RegisterWeapon("LaserGun", std::make_unique<BossMechWeaponLaserGun>(this));

	// ステートテーブル作成
	states_[BossMech::BossMechState::Idle] = std::make_unique<BossMechStateIdle>();
	states_[BossMech::BossMechState::LaserShot] = std::make_unique<BossMechStateLaserShot>();

	// 最初のステートを設定
	ChangeState(BossMechState::Idle);
}


void BossMech::Update([[maybe_unused]] bool isShowDebugUI, [[maybe_unused]] const BaseMech::InitParam& param) {
	// デバッグの更新
	DebugUpdate(isShowDebugUI, param);

	// ステート更新
	if (auto& state = currentState_.second) {
		state->Update(this);
	}
	// 基底クラスの更新
	BaseMech::Update(isShowDebugUI, param);
}

void BossMech::ChangeState(BossMech::BossMechState nextState) {
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


BossMechBaseState* BossMech::GetState(BossMech::BossMechState state) {
	// ステートテーブルから検索
	auto it = states_.find(state);
	if (it != states_.end()) {
		return it->second.get();
	}

	MAGIAssert::Assert(false, "Not find BossMechState!");
	return {};
}

const std::string BossMech::StateToString(BossMech::BossMechState state) {
	switch (state) {
		case BossMech::BossMechState::Idle:
			return "Idle";
		case BossMech::BossMechState::LaserShot:
			return "LaserShot";
		default:
			return "Unknown";
	}
}

const std::string BossMech::TransTypeToString(MechAnimation::TransType partsType) {
	switch (partsType) {
		case MechAnimation::TransType::Head:
			return "Head";
		case MechAnimation::TransType::Body:
			return "Body";

		case MechAnimation::TransType::UpperArmLeft:
			return "UpperArmLeft";
		case MechAnimation::TransType::LowerArmLeft:
			return "LowerArmLeft";
		case MechAnimation::TransType::HandLeft:
			return "HandLeft";

		case MechAnimation::TransType::UpperArmRight:
			return "UpperArmRight";
		case MechAnimation::TransType::LowerArmRight:
			return "LowerArmRight";
		case MechAnimation::TransType::HandRight:
			return "HandRight";

		case MechAnimation::TransType::Waist:
			return "Waist";

		case MechAnimation::TransType::UpperLegLeft:
			return "UpperLegLeft";
		case MechAnimation::TransType::LowerLegLeft:
			return "LowerLegLeft";
		case MechAnimation::TransType::FootLeft:
			return "FootLeft";

		case MechAnimation::TransType::UpperLegRight:
			return "UpperLegRight";
		case MechAnimation::TransType::LowerLegRight:
			return "LowerLegRight";
		case MechAnimation::TransType::FootRight:
			return "FootRight";

		default:
			return "Unknown";
	}
}

void BossMech::ShowDebugWindow() {
	// デバッグ操作ウィンドウ
	ImGui::Begin("BossMech");

	ImGui::SeparatorText("Parameter");
	{
		// 現在ステート表示
		ImGui::Text("CurrentState :");
		ImGui::SameLine();
		const std::string state = StateToString(currentState_.first);
		ImGui::Text(state.c_str());
	}

	ImGui::SeparatorText("SwitchState");
	{
		// 箱の高さ
		const float boxHeight = 100.0f;

		// スクロールできる箱（Child）
		ImGui::BeginChild("SwitchStateBox", ImVec2(0.0f, boxHeight), true);

		int index = 0;
		for (const auto& [stateEnum, statePtr] : states_) {
			(void)statePtr;

			// ID衝突対策
			ImGui::PushID(index++);

			// ステート切り替えボタン
			const std::string stateName = StateToString(stateEnum);
			if (ImGui::Button(stateName.c_str(), ImVec2(-1.0f, 0.0f))) {
				ChangeState(stateEnum);
			}

			ImGui::PopID();
		}

		ImGui::EndChild();
	}

	// デバッグフラグのUI
	ShowDebugFlagUI();

	ImGui::End();
}

