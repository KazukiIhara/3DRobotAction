#include "BossMech.h"

#include "MAGI.h"
#include "MAGIAssert/MAGIAssert.h" 

#include "GameObject/Mech/MechCore/MechCore.h"
#include "GameObject/Damage/Object/Manager/DamageObjectManager.h"
#include "GameEffects/System/GameEffectManager/GameEffectManager.h"


// ステートクラス
#include "GameObject/Boss/Mech/State/Idle/BossMechStateIdle.h"
#include "GameObject/Boss/Mech/State/LaserShot/BossMechStateLaserShot.h"

using namespace Magi;

BossMech::BossMech(const BossMech::InitParam& initParam, DamageObjectManager* damageObjectManager, GameEffectManager* gameEffectManager, MechCore* playerMech) {
	// 参照ポインタを受け取る
	damageObjectManager_ = damageObjectManager;
	gameEffectManager_ = gameEffectManager;
	playerMech_ = playerMech;

	// トランスフォーム作成
	std::unique_ptr<Transform3D> trans = std::make_unique<Transform3D>(initParam.position);
	transform_ = MAGISYSTEM::AddTransform3D(std::move(trans));
	{
		// パーツをマップ追加
		using BP = BossMech::PartsType;
		parts_[BP::Head] = std::make_unique<BossMechHead>(initParam.head, this);
		parts_[BP::Body] = std::make_unique<BossMechBody>(initParam.body, this);
		parts_[BP::ArmR] = std::make_unique<BossMechRightArm>(initParam.arm, this);
		parts_[BP::ArmL] = std::make_unique<BossMechLeftArm>(initParam.arm, this);
		parts_[BP::LegR] = std::make_unique<BossMechRightLeg>(initParam.leg, this);
		parts_[BP::LegL] = std::make_unique<BossMechLeftLeg>(initParam.leg, this);

	}

	// 武器をマップに追加
	weapons_["LaserGun"] = std::make_unique<BossMechWeaponLaserGun>(this);

	// 実装メモ
	/*
		胴体のみこのBossMech(コア)と親子付け
		そのほかのパーツはBodyのトランスフォームを親とする
		各パーツ内のさらに細かいパーツはパーツ作成時に親子付けする
	*/

	// ステートテーブル作成
	states_[BossMech::BossMechState::Idle] = std::make_unique<BossMechStateIdle>();
	states_[BossMech::BossMechState::LaserShot] = std::make_unique<BossMechStateLaserShot>();

	// 最初のステートを設定
	ChangeState(BossMechState::Idle);

}

void BossMech::Update(bool isShowDebugUI) {
#if defined (DEBUG) | (DEVELOP)
	// デバッグUIを表示
	if (isShowDebugUI) {
		DebugDraw();
	}
#endif

	// ステート更新
	if (auto& state = currentState_.second) {
		state->Update(this);
	}

	// 全パーツを更新
	for (auto& part : parts_) {
		part.second->Update();
	}

	// 全武器を更新
	for (auto& weapon : weapons_) {
		weapon.second->Update();
	}

}

void BossMech::Draw() {
	// 全パーツを描画
	for (auto& part : parts_) {
		part.second->Draw();
	}

	// 全武器を描画
	for (auto& weapon : weapons_) {
		weapon.second->Draw();
	}

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

BossMechBaseWeapon* BossMech::GetWeapon(const std::string& name) {
	// 名前で武器を検索
	auto it = weapons_.find(name);
	if (it == weapons_.end()) {
		return nullptr;
	}
	return it->second.get();
}

MechCore* BossMech::GetPlayerMech() {
	return playerMech_;
}

DamageObjectManager* BossMech::GetDamageObjectManager() {
	return damageObjectManager_;
}

GameEffectManager* BossMech::GetGameEffectManager() {
	return gameEffectManager_;
}

void BossMech::DebugDraw() {
	// デバッグウィンドウ描画処理
	ShowDebugWidow();

	// 各パーツのデバッグ描画
	for (auto& part : parts_) {
		// パーツデバッグ描画
		if (debugFlag_.showPartsTransform) {
			part.second->DebugDraw();
		}
	}
}

BossMechBaseState* BossMech::GetState(BossMech::BossMechState state) {
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
			break;
		case BossMech::BossMechState::LaserShot:
			return "LaserShot";
		default:
			return "Unknown";
	}
}

const std::string BossMech::PartsTypeToString(BossMech::PartsType partsType) {
	// パーツタイプを文字列に変換
	switch (partsType) {
		case BossMech::PartsType::Head:
			return "Head";
		case BossMech::PartsType::Body:
			return "Body";
		case BossMech::PartsType::ArmR:
			return "RightArm";
		case BossMech::PartsType::ArmL:
			return "LeftArm";
		case BossMech::PartsType::LegR:
			return "RightLeg";
		case BossMech::PartsType::LegL:
			return "LeftLeg";
		default:
			return "Unknown";
	}
}
void BossMech::ShowDebugWidow() {
	// デバッグ操作ウィンドウ
	ImGui::Begin("BossMech");

	ImGui::SeparatorText("Parameter");
	{
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

			ImGui::PushID(index++); // ID衝突対策

			// ボタンラベル
			const std::string stateName = StateToString(stateEnum);

			// ステート切り替えボタン
			if (ImGui::Button(stateName.c_str(), ImVec2(-1.0f, 0.0f))) {
				ChangeState(stateEnum);
			}

			ImGui::PopID();
		}

		ImGui::EndChild();
	}

	ImGui::SeparatorText("DebugFlag");
	if (ImGui::Button("SwitchPartsDebug")) {
		SwitchShowPartsTransform();
	}


	ImGui::End();
}

void BossMech::SwitchShowPartsTransform() {
	debugFlag_.showPartsTransform = !debugFlag_.showPartsTransform;
}
