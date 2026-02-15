#include "PilotMech.h"

#include "MAGI.h"
#include "MAGIAssert/MAGIAssert.h"

// マネージャの参照用
#include "Feature/Damage/Object/Manager/DamageObjectManager.h"
#include "Feature/Effect/System/GameEffectManager/GameEffectManager.h"
#include "MechAnimation/Container/MechAnimationContainer.h"

// ステートクラス
#include "Feature/Pilot/Mech/State/Idle/PilotMechStateIdle.h"
#include "Feature/Pilot/Mech/State/Move/PilotMechStateMove.h"
#include "Feature/Pilot/Mech/State/Dodge/PilotMechStateDodge.h"
#include "Feature/Pilot/Mech/State/JustDodge/PilotMechStateJustDodge.h"
#include "Feature/Pilot/Mech/State/JustDodgeAttack/PilotMechStateJustDodgeAttack.h"
#include "Feature/Pilot/Mech/State/HitReact/PilotMechStateHitReact.h"
#include "Feature/Pilot/Mech/State/KnockBack/PilotMechStateKnockBack.h"

// 武器
#include "Feature/Pilot/Mech/Weapon/BeamCannonRifle/PilotMechWeaponBeamCannonRifle.h"
#include "Feature/Pilot/Mech/Weapon/MachineGun/PilotMechWeaponMachineGun.h"

using namespace Magi;

// 
// 以下改修予定
//

// ボス機体
#include "Feature/Boss/Mech/BossMech.h"

PilotMech::PilotMech(const InitParam& param, const BaseMech::RefContext& ref, GameInputSystem* inputSys)
	:BaseMech(param, ref) {

	inputSys_ = inputSys;

	// 追加パーツがあればここに追加

	// 武器をマップに追加
	// ビームキャノン
	AddWeapon("BeamCannonRifle", std::make_unique<PilotMechWeaponBeamCannonRifle>(this));
	// マシンガン
	AddWeapon("MachineGun", std::make_unique<PilotMechWeaponMachineGun>(this));

	// ジャスト回避コライダー
	justDodgeCollider_ = std::make_unique<PilotMechJustDodgeCollider>(this);

	// ロックオンシステム
	lockOnSystem_ = std::make_unique<PilotMechLockOnSystem>(this);

	// 上下移動システム
	verticalMoveSystem_ = std::make_unique<PilotMechVerticalMoveSystem>(this);

	// ステータス
	status_ = std::make_unique<PilotMechStatus>(this);

	// ステートテーブル作成
	states_[State::Idle] = std::make_unique<PilotMechStateIdle>();
	states_[State::Move] = std::make_unique<PilotMechStateMove>();
	states_[State::Dodge] = std::make_unique<PilotMechStateDodge>();
	states_[State::JustDodge] = std::make_unique<PilotMechStateJustDodge>();
	states_[State::JustDodgeAttack] = std::make_unique<PilotMechStateJustDodgeAttack>();
	states_[State::HitReact] = std::make_unique<PilotMechStateHitReact>();
	states_[State::KnockBack] = std::make_unique<PilotMechStateKnockBack>();

	// 最初のステートを設定
	ChangeState(State::Idle);

}

void PilotMech::Update([[maybe_unused]] bool isShowDebugUI, [[maybe_unused]] const BaseMech::InitParam& param) {
	// デバッグの更新
	DebugUpdate(isShowDebugUI, param);

	// 基本的に進行方向に機体を向ける 向けたくない場合はステートごとにこのフラグを切る
	GetRotControlSystem()->SetTurnToMoveDir(true);
	// 腰回転アニメーションを有効にする(切りたい場合はステートごとに切る)

	// ロックオンシステム更新
	lockOnSystem_->Update();

	// ステータス更新
	status_->Update();

	// ステート更新
	if (auto& state = currentState_.second) {
		state->Update(this);
	}

	// 上下移動クラス更新
	verticalMoveSystem_->PreUpdate();

	// 基底クラスの更新
	BaseMech::Update(isShowDebugUI, param);

	// 戦闘アニメーション制御システム


	// 簡易IK、機体回転、コライダー更新
	BaseMech::PreUpdate();

	// 上下移動クラス　機体の移動量確定後の処理
	verticalMoveSystem_->PostUpdate();

	// コライダー更新
	justDodgeCollider_->Update();
}

void PilotMech::DebugDraw() {
	BaseMech::DebugDraw();
	if (GetDebugFlag().isDebugDraw) {
		justDodgeCollider_->Draw();
	}
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

PilotMechJustDodgeCollider* PilotMech::GetJustDodgeCollider() {
	return justDodgeCollider_.get();
}

PilotMechLockOnSystem* PilotMech::GetLockOnSystem() {
	return lockOnSystem_.get();
}

PilotMechVerticalMoveSystem* PilotMech::GetVerticalMoveSystem() {
	return verticalMoveSystem_.get();
}

PilotMechStatus* PilotMech::GetStatus() {
	return status_.get();
}

GameInputSystem* PilotMech::GetInputSys() {
	return inputSys_;
}

void PilotMech::SetBossMech(BossMech* mech) {
	lockOnSystem_->SetBoss(mech);
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
