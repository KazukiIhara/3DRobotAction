#include "BossMech.h"

#include "MAGI.h"
#include "MAGIAssert/MAGIAssert.h"

#include "Feature/Damage/Object/Manager/DamageObjectManager.h"
#include "Feature/Effect/System/GameEffectManager/GameEffectManager.h"
#include "MechAnimation/Container/MechAnimationContainer.h"

// ステートクラス
#include "Feature/Boss/Mech/State/Idle/BossMechStateIdle.h"
#include "Feature/Boss/Mech/State/LaserShot/BossMechStateLaserShot.h"

// 武器クラス
#include "Feature/Boss/Mech/Weapon/LaserGun/BossMechWeaponLaserGun.h"

// プレイヤー機体
#include "Feature/Pilot/Mech/PilotMech.h"

using namespace Magi;

BossMech::BossMech(const InitParam& param, const RefContext& ref, PilotMech* pilotMech) :
	BaseMech(param, ref) {

	// プレイヤー機体の参照ポインタを受け取る
	pilotMech_ = pilotMech;

	// 追加パーツがあればここに追加

	// バリア作成
	barrier_ = std::make_unique<BossMechBarrier>(this);
	// パーツとして追加
	AddParts(barrier_.get());

	// 武器をマップに追加
	AddWeapon("LaserGun", std::make_unique<BossMechWeaponLaserGun>(this));

	{
		// ステータスを初期化
		BossMechStatus::InitParam sInitParam{};
		sInitParam.hp = param.hp;
		status_ = std::make_unique<BossMechStatus>(sInitParam, this);
	}

	// ステートテーブル作成
	states_[State::Idle] = std::make_unique<BossMechStateIdle>();
	states_[State::LaserShot] = std::make_unique<BossMechStateLaserShot>();

	// 最初のステートを設定
	ChangeState(State::Idle);
}


void BossMech::Update([[maybe_unused]] bool isShowDebugUI, [[maybe_unused]] const BaseMech::InitParam& param) {
	// デバッグの更新
	DebugUpdate(isShowDebugUI, param);

	// パイロット機体をターゲットにセット
	const Vector3 pilotCenter = GetPilotMech()->GetCenterPos();
	SetTargetWorldPos(pilotCenter);

	// ステータスの更新
	status_->Update();

	// バリアの更新
	barrier_->Update();

	// ステート更新
	if (auto& state = currentState_.second) {
		state->Update(this);
	}
	// 基底クラスの更新
	BaseMech::Update(isShowDebugUI, param);
}

void BossMech::ChangeState(BossMech::State nextState) {
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

BossMechBarrier* BossMech::GetBarrier() {
	return barrier_.get();
}

BossMechStatus* BossMech::GetStatus() {
	return status_.get();
}

PilotMech* BossMech::GetPilotMech() {
	return pilotMech_;
}

BossMechBaseState* BossMech::GetState(BossMech::State state) {
	// ステートテーブルから検索
	auto it = states_.find(state);
	if (it != states_.end()) {
		return it->second.get();
	}

	MAGIAssert::Assert(false, "Not find BossMechState!");
	return {};
}

const std::string BossMech::StateToString(BossMech::State state) {
	switch (state) {
		case BossMech::State::Idle:
			return "Idle";
		case BossMech::State::LaserShot:
			return "LaserShot";
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
		// HP表示
		ImGui::Text("HP: ");
		ImGui::SameLine();
		int32_t hp = status_->GetHP();
		ImGui::Text("%u", hp);
	}

	ImGui::SeparatorText("LookAtFlag");
	{
		auto& look = GetRotControlSystem()->GetLookAtFlag();
		ImGui::Checkbox("MechYaw", &look.mech.yaw);
		ImGui::Checkbox("MechPitch", &look.mech.pitch);
		ImGui::Checkbox("BodyYaw", &look.body.yaw);
		ImGui::Checkbox("BodyPitch", &look.body.pitch);
		ImGui::Checkbox("HeadYaw", &look.head.yaw);
		ImGui::Checkbox("HeadPitch", &look.head.pitch);
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

