#include "BaseMech.h"

#include "MAGI.h"
#include "3D/Transform3D/Transform3D.h"

#include "Feature/Damage/Object/Manager/DamageObjectManager.h"
#include "Feature/Effect/System/GameEffectManager/GameEffectManager.h"
#include "MechAnimation/Container/MechAnimationContainer.h"
#include "Feature/Mech/Weapon/BaseMechWeapon.h"

using namespace Magi;

BaseMech::BaseMech(const InitParam& param, const RefContext& ref) {
	tag_ = param.tag;
	ref_ = ref;

	// ルートトランスフォーム
	std::unique_ptr<Transform3D> trans = std::make_unique<Transform3D>(param.position);
	transform_ = MAGISYSTEM::AddTransform3D(std::move(trans));

	// 描画用トランスフォーム
	modelTransform_ = MAGISYSTEM::AddTransform3D();
	modelTransform_->SetParent(transform_, false);

	// パーツ作成
	CreateParts(param);
	// 一応パラメータ初期化
	SetInitParam(param);
	// 関節配列初期化
	partsTrans_.fill(nullptr);

	// パーツリストと関節配列作成
	PartsSetUp();

	// アニメーター
	animator_ = std::make_unique<MechAnimator>(ref_.animationContainer, this);
	// 移動制御クラス作成
	moveSystem_ = std::make_unique<MechMoveSystem>(this);
	// 回転制御クラス作成
	rotControlSystem_ = std::make_unique<MechRotControlSystem>(this);
	// コライダー作成
	collider_ = std::make_unique<MechCollider>(this);

}

void BaseMech::Update([[maybe_unused]] bool isShowDebugUI, [[maybe_unused]] const BaseMech::InitParam& param) {
	// パーツ更新
	for (auto& p : parts_) {
		p->Update();
	}

	// 武器更新
	for (auto& kv : weapons_) {
		if (kv.second) {
			kv.second->Update();
		}
	}

	// 機体の速度を計算
	if (moveSystem_) {
		moveSystem_->CalSpeed();
	}

	// 地形追加後　多分ここで押し戻し判定を取る


	// 移動量を計算して追加
	if (moveSystem_) {
		moveSystem_->ApplyVelocity();
	}

	// 機体の回転を更新
	if (rotControlSystem_) {
		rotControlSystem_->Update();
	}

	// コライダー更新
	if (collider_) {
		collider_->Update();
	}
}

void BaseMech::Draw() {
	// パーツ描画
	for (auto& p : parts_) {
		p->Draw();
	}

	// 武器描画
	for (auto& kv : weapons_) {
		if (kv.second) {
			kv.second->Draw();
		}
	}

}

void BaseMech::DebugDraw() {
	// デバッグウィンドウ描画処理
	ShowDebugWindow();

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
		// コライダー描画
		if (collider_) {
			collider_->Draw();
		}
	}
}

void BaseMech::DebugUpdate(bool isShowDebugUI, const BaseMech::InitParam& param) {
#if defined(DEBUG) || defined(DEVELOP)
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
}

Transform3D* BaseMech::GetTransform() {
	return transform_;
}

Transform3D* BaseMech::GetModelTransform() {
	return modelTransform_;
}

MechPartsHead* BaseMech::GetHead() {
	return head_.get();
}
MechPartsBody* BaseMech::GetBody() {
	return body_.get();
}
MechPartsArm* BaseMech::GetArmL() {
	return armL_.get();
}
MechPartsArm* BaseMech::GetArmR() {
	return armR_.get();
}
MechPartsLeg* BaseMech::GetLeg() {
	return leg_.get();
}

Transform3D* BaseMech::GetPartsTransform(MechAnimation::TransType type) {
	const size_t index = static_cast<size_t>(type);
	if (index >= partsTrans_.size()) {
		return nullptr;
	}
	return partsTrans_[index];
}

const Vector3& BaseMech::GetCenterPos() {
	if (body_) {
		return body_->GetTransform()->GetWorldPosition();
	}
	return Vector3{};
}

BaseMechWeapon* BaseMech::GetWeapon(const std::string& name) {
	auto it = weapons_.find(name);
	if (it == weapons_.end()) {
		return nullptr;
	}
	return it->second.get();
}

MechAnimator* BaseMech::GetAnimator() {
	return animator_.get();
}

MechMoveSystem* BaseMech::GetMoveSystem() {
	return moveSystem_.get();
}

MechRotControlSystem* BaseMech::GetRotControlSystem() {
	return rotControlSystem_.get();
}

MechCollider* BaseMech::GetCollider() {
	return collider_.get();
}

FriendlyTag BaseMech::GetTag()const {
	return tag_;
}

void BaseMech::SetTargetWorldPos(const Vector3& targetWorldPos) {
	targetWorldPos_ = targetWorldPos;
}

const Vector3& BaseMech::GetTargetWorldPos() const {
	return targetWorldPos_;
}

const std::string BaseMech::TransTypeToString(MechAnimation::TransType partsType) {
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


BaseMech::DebugFlag& BaseMech::GetDebugFlag() {
	return debugFlag_;
}
const BaseMech::DebugFlag& BaseMech::GetDebugFlag() const {
	return debugFlag_;
}

DamageObjectManager* BaseMech::GetDamageObjectManager() {
	return ref_.damageObjectManager;
}
DamageCollisionSystem* BaseMech::GetDamageCollisionSystem() {
	return ref_.damageCollisionSystem;
}
GameEffectManager* BaseMech::GetGameEffectManager() {
	return ref_.gameEffectManager;
}
MechAnimationContainer* BaseMech::GetAnimationContainer() {
	return ref_.animationContainer;
}

void BaseMech::SetInitParam(const BaseMech::InitParam& initParam) {
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

void BaseMech::AddParts(IMechParts* parts) {
	parts_.push_back(parts);
}

void BaseMech::RegisterWeapon(const std::string& name, std::unique_ptr<BaseMechWeapon> weapon) {
	if (name.empty() || !weapon) {
		return;
	}
	weapons_[name] = std::move(weapon);
}


void BaseMech::ShowDebugFlagUI() {
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
}

void BaseMech::CreateParts(const InitParam& param) {
	// 生成順が重要（親参照があるため）
	leg_ = std::make_unique<MechPartsLeg>(param.leg, this);
	body_ = std::make_unique<MechPartsBody>(param.body, this);
	head_ = std::make_unique<MechPartsHead>(param.head, this);
	armR_ = std::make_unique<MechPartsArm>(param.armR, this);
	armL_ = std::make_unique<MechPartsArm>(param.armL, this);
}

void BaseMech::PartsSetUp() {
	// パーツリスト構築
	BuildPartsList();

	// 関節配列構築
	BuildPartsTransformArray();
}

void BaseMech::BuildPartsList() {
	parts_.clear();

	parts_.push_back(head_.get());
	parts_.push_back(body_.get());
	parts_.push_back(armL_.get());
	parts_.push_back(armR_.get());
	parts_.push_back(leg_.get());
}

void BaseMech::BuildPartsTransformArray() {
	// 配列初期化
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

void BaseMech::SwitchShowPartsTransform() {
	// 表示フラグ反転
	debugFlag_.isDebugDraw = !debugFlag_.isDebugDraw;
}

void BaseMech::SwitchEditPartsTransform() {
	// 編集フラグ反転
	debugFlag_.editPartsTransform = !debugFlag_.editPartsTransform;
}

void BaseMech::SwitchStopUpdate() {
	debugFlag_.stopUpdate = !debugFlag_.stopUpdate;
}
