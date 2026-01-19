#include "BossMech.h"

#include "MAGI.h"
#include "MAGIAssert/MAGIAssert.h"

#include "GameObject/Mech/MechCore/MechCore.h"
#include "GameObject/Damage/Object/Manager/DamageObjectManager.h"
#include "GameEffects/System/GameEffectManager/GameEffectManager.h"
#include "MechAnimation/Container/MechAnimationContainer.h"

// ステートクラス
#include "GameObject/Boss/Mech/State/Idle/BossMechStateIdle.h"
#include "GameObject/Boss/Mech/State/LaserShot/BossMechStateLaserShot.h"

using namespace Magi;

BossMech::BossMech(
	const BossMech::InitParam& initParam,
	DamageObjectManager* damageObjectManager,
	GameEffectManager* gameEffectManager,
	MechAnimationContainer* mechAnimationContainer,
	MechCore* playerMech
) {
	// 参照ポインタを受け取る
	damageObjectManager_ = damageObjectManager;
	gameEffectManager_ = gameEffectManager;
	mechAnimationContainer_ = mechAnimationContainer;
	playerMech_ = playerMech;

	// ルートトランスフォーム作成
	std::unique_ptr<Transform3D> trans = std::make_unique<Transform3D>(initParam.position);
	transform_ = MAGISYSTEM::AddTransform3D(std::move(trans));

	// パーツを作成（Bodyが親になる想定）
	leg_ = std::make_unique<BossMechLeg>(initParam.leg, this);
	body_ = std::make_unique<BossMechBody>(initParam.body, this);
	head_ = std::make_unique<BossMechHead>(initParam.head, this);
	armR_ = std::make_unique<BossMechRightArm>(initParam.armR, this);
	armL_ = std::make_unique<BossMechLeftArm>(initParam.armL, this);

	// パーツをリストに追加
	parts_.push_back(head_.get());
	parts_.push_back(body_.get());
	parts_.push_back(armR_.get());
	parts_.push_back(armL_.get());
	parts_.push_back(leg_.get());

	// 関節トランスフォーム配列作成
	CreatePartsTransformArray();

	// 武器をマップに追加
	weapons_["LaserGun"] = std::make_unique<BossMechWeaponLaserGun>(this);

	// ステートテーブル作成
	states_[BossMech::BossMechState::Idle] = std::make_unique<BossMechStateIdle>();
	states_[BossMech::BossMechState::LaserShot] = std::make_unique<BossMechStateLaserShot>();

	// アニメーションクラスを作成
	animator_ = std::make_unique<MechAnimator>(mechAnimationContainer_, this);

	// コライダーを作成
	collider_ = std::make_unique<BossMechCollider>(this);

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

BossMechHead* BossMech::GetHead() {
	return head_.get();
}

BossMechBody* BossMech::GetBody() {
	return body_.get();
}

BossMechRightArm* BossMech::GetRightArm() {
	return armR_.get();
}

BossMechLeftArm* BossMech::GetLeftArm() {
	return armL_.get();
}

BossMechLeg* BossMech::GetLeg() {
	return leg_.get();
}

Transform3D* BossMech::GetPartsTransform(MechAnimation::TransType type) {
	// 配列境界チェック
	const size_t index = static_cast<size_t>(type);
	if (index >= partsTrans_.size()) {
		return nullptr;
	}
	return partsTrans_[index];
}

BossMechBaseWeapon* BossMech::GetWeapon(const std::string& name) {
	// 名前で武器を検索
	auto it = weapons_.find(name);
	if (it == weapons_.end()) {
		return nullptr;
	}
	return it->second.get();
}

MechAnimator* BossMech::GetAnimator() {
	return animator_.get();
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

void BossMech::CreatePartsTransformArray() {
	// 全要素を初期化
	partsTrans_.fill(nullptr);

	// Head / Body
	if (head_) {
		partsTrans_[static_cast<size_t>(MechAnimation::TransType::Head)] = head_->GetHeadTransform();
	}
	if (body_) {
		partsTrans_[static_cast<size_t>(MechAnimation::TransType::Body)] = body_->GetTransform();
	}

	// Arm Left
	if (armL_) {
		partsTrans_[static_cast<size_t>(MechAnimation::TransType::UpperArmLeft)] = armL_->GetUpperTransform();
		partsTrans_[static_cast<size_t>(MechAnimation::TransType::LowerArmLeft)] = armL_->GetLowerTransform();
		partsTrans_[static_cast<size_t>(MechAnimation::TransType::HandLeft)] = armL_->GetHandTransform();
	}

	// Arm Right
	if (armR_) {
		partsTrans_[static_cast<size_t>(MechAnimation::TransType::UpperArmRight)] = armR_->GetUpperTransform();
		partsTrans_[static_cast<size_t>(MechAnimation::TransType::LowerArmRight)] = armR_->GetLowerTransform();
		partsTrans_[static_cast<size_t>(MechAnimation::TransType::HandRight)] = armR_->GetHandTransform();
	}

	// Leg
	if (leg_) {
		partsTrans_[static_cast<size_t>(MechAnimation::TransType::Waist)] = leg_->GetWaistTransform();

		partsTrans_[static_cast<size_t>(MechAnimation::TransType::UpperLegLeft)] = leg_->GetUpperTransformLeft();
		partsTrans_[static_cast<size_t>(MechAnimation::TransType::LowerLegLeft)] = leg_->GetLowerTransformLeft();
		partsTrans_[static_cast<size_t>(MechAnimation::TransType::FootLeft)] = leg_->GetFootTransformLeft();

		partsTrans_[static_cast<size_t>(MechAnimation::TransType::UpperLegRight)] = leg_->GetUpperTransformRight();
		partsTrans_[static_cast<size_t>(MechAnimation::TransType::LowerLegRight)] = leg_->GetLowerTransformRight();
		partsTrans_[static_cast<size_t>(MechAnimation::TransType::FootRight)] = leg_->GetFootTransformRight();
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
