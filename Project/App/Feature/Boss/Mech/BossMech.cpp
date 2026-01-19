#include "BossMech.h"

#include "MAGI.h"
#include "MAGIAssert/MAGIAssert.h"

#include "GameObject/Mech/MechCore/MechCore.h"
#include "GameObject/Damage/Object/Manager/DamageObjectManager.h"
#include "GameEffects/System/GameEffectManager/GameEffectManager.h"
#include "MechAnimation/Container/MechAnimationContainer.h"

// ステートクラス
#include "Feature/Boss/Mech/State/Idle/BossMechStateIdle.h"
#include "Feature/Boss/Mech/State/LaserShot/BossMechStateLaserShot.h"

using namespace Magi;

BossMech::BossMech(const InitParam& param, const RefContext& ref, MechCore* playerMech) :
	BaseMech(param, ref) {
	playerMech_ = playerMech;

	// 武器をマップに追加
	RegisterWeapon("LaserGun", std::make_unique<BossMechWeaponLaserGun>(this));

	// ステートテーブル作成
	states_[BossMech::BossMechState::Idle] = std::make_unique<BossMechStateIdle>();
	states_[BossMech::BossMechState::LaserShot] = std::make_unique<BossMechStateLaserShot>();

	// 最初のステートを設定
	ChangeState(BossMechState::Idle);
}

void BossMech::Update([[maybe_unused]] bool isShowDebugUI, [[maybe_unused]] const BossMech::InitParam& param) {
#if defined(DEBUG) || defined(DEVELOP)
	// デバッグUIを表示
	if (isShowDebugUI) {
		DebugDraw();
	}
	// デバッグ編集が有効なら初期化値を反映
	if (debugFlag_.editPartsTransform) {
		SetInitParam(param);
	}
	// 更新停止フラグ
	if (debugFlag_.stopUpdate) {
		return;
	}
#endif

	// ステート更新
	if (auto& state = currentState_.second) {
		state->Update(this);
	}

	// 全パーツを更新
	for (auto& part : parts_) {
		part->Update();
	}

	// 全武器を更新
	for (auto& weapon : weapons_) {
		weapon.second->Update();
	}

	// コライダーを更新
	collider_->Update();

}

void BossMech::Draw() {
	// 全パーツを描画
	for (auto& part : parts_) {
		part->Draw();
	}

	// 全武器を描画
	for (auto& weapon : weapons_) {
		weapon.second->Draw();
	}

	// コライダーを描画
	collider_->Draw();
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

Transform3D* BossMech::GetTransform() {
	return transform_;
}

Transform3D* BossMech::GetPartsTransform(MechAnimation::TransType type) {
	// 配列境界チェック
	const size_t index = static_cast<size_t>(type);
	if (index >= partsTrans_.size()) {
		return nullptr;
	}
	return partsTrans_[index];
}

MechAnimator* BossMech::GetAnimator() {
	return animator_.get();
}

MechCore* BossMech::GetPlayerMech() {
	return playerMech_;
}

void BossMech::DebugDraw() {
	// デバッグウィンドウ描画処理
	ShowDebugWidow();
	// パーツデバッグ描画
	if (debugFlag_.isDebugDraw) {
		// 各パーツのデバッグ描画
		for (auto& part : parts_) {
			part->DebugDraw();
		}
		// 武器のデバッグ描画
		for (auto& w : weapons_) {
			w.second->DebugDraw();
		}
	}

}

void BossMech::SetInitParam(const BossMech::InitParam& initParam) {
	// 頭の初期値反映
	if (head_) {
		head_->SetInitTranslate(initParam.head);
	}
	// 胴体の初期値反映
	if (body_) {
		body_->SetInitTranslate(initParam.body);
	}
	// 右腕の初期値反映
	if (armR_) {
		armR_->SetInitTranslate(initParam.armR);
	}
	// 左腕の初期値反映
	if (armL_) {
		armL_->SetInitTranslate(initParam.armL);
	}
	// 足の初期値反映
	if (leg_) {
		leg_->SetInitTranslate(initParam.leg);
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

void BossMech::ShowDebugWidow() {
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

	ImGui::SeparatorText("DebugFlag");
	{
		// 更新停止フラグ切り替え
		if (ImGui::Button("StopUpdate")) {
			SwitchStopUpdate();
		}
		// パーツのデバッグ描画切り替え
		if (ImGui::Button("ShowPartsDebug")) {
			SwitchShowPartsTransform();
		}
		// パーツ編集モード切り替え
		if (ImGui::Button("EditPartsTrans")) {
			SwitchEditPartsTransform();
		}
	}

	ImGui::End();
}

void BossMech::SwitchShowPartsTransform() {
	// 表示フラグ反転
	debugFlag_.isDebugDraw = !debugFlag_.isDebugDraw;
}

void BossMech::SwitchEditPartsTransform() {
	// 編集フラグ反転
	debugFlag_.editPartsTransform = !debugFlag_.editPartsTransform;
}

void BossMech::SwitchStopUpdate() {
	debugFlag_.stopUpdate = !debugFlag_.stopUpdate;
}
