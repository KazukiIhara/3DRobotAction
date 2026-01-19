#include "BaseMech.h"

#include "MAGI.h"
#include "3D/Transform3D/Transform3D.h"

#include "GameObject/Damage/Object/Manager/DamageObjectManager.h"
#include "GameEffects/System/GameEffectManager/GameEffectManager.h"
#include "MechAnimation/Container/MechAnimationContainer.h"
#include "Feature/Mech/Weapon/BaseMechWeapon.h"

using namespace Magi;

BaseMech::BaseMech(const InitParam& param, const RefContext& ref) {
	tag_ = param.tag;
	ref_ = ref;

	// ルートトランスフォーム
	std::unique_ptr<Transform3D> trans = std::make_unique<Transform3D>(param.position);
	transform_ = MAGISYSTEM::AddTransform3D(std::move(trans));

	// パーツ作成
	CreateParts(param);

	// 関節配列初期化
	partsTrans_.fill(nullptr);

	// パーツリストと関節配列作成
	PartsSetUp();

	// アニメーター
	if (ref_.animationContainer) {
		animator_ = std::make_unique<MechAnimator>(ref_.animationContainer, this);
	}

	// コライダー作成
	collider_ = std::make_unique<MechCollider>(this);

}

void BaseMech::Update(bool isShowDebugUI) {
#if defined(DEBUG) || defined(DEVELOP)
	if (isShowDebugUI) {
		DebugDraw();
	}
	if (debugFlag_.stopUpdate) {
		return;
	}
#endif

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

	// コライダー描画
	if (collider_) {
		collider_->Draw();
	}
}

Transform3D* BaseMech::GetTransform() {
	return transform_;
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


BaseMech::DebugFlag& BaseMech::GetDebugFlag() {
	return debugFlag_;
}
const BaseMech::DebugFlag& BaseMech::GetDebugFlag() const {
	return debugFlag_;
}

DamageObjectManager* BaseMech::GetDamageObjectManager() {
	return ref_.damageObjectManager;
}
GameEffectManager* BaseMech::GetGameEffectManager() {
	return ref_.gameEffectManager;
}
MechAnimationContainer* BaseMech::GetAnimationContainer() {
	return ref_.animationContainer;
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

void BaseMech::DebugDraw() {
	ShowDebugWindow();
}
