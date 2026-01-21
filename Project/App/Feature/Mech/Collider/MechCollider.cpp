#include "MechCollider.h"

#include "Feature/Mech/Base/BaseMech.h"

#include "MAGI.h"

using namespace Magi;

MechCollider::MechCollider(BaseMech* mech) {
	mech_ = mech;
}

void MechCollider::Update() {
	if (!mech_) {
		return;
	}

	// 頭〜腰
	SetCapsule(0, MechAnimation::TransType::Head, MechAnimation::TransType::Waist);

	// 左腕：上腕〜肘、肘〜手
	SetCapsule(1, MechAnimation::TransType::UpperArmLeft, MechAnimation::TransType::LowerArmLeft);
	SetCapsule(2, MechAnimation::TransType::LowerArmLeft, MechAnimation::TransType::HandLeft);

	// 右腕：上腕〜肘、肘〜手
	SetCapsule(3, MechAnimation::TransType::UpperArmRight, MechAnimation::TransType::LowerArmRight);
	SetCapsule(4, MechAnimation::TransType::LowerArmRight, MechAnimation::TransType::HandRight);

	// 左脚：上足〜膝、膝〜足
	SetCapsule(5, MechAnimation::TransType::UpperLegLeft, MechAnimation::TransType::LowerLegLeft);
	SetCapsule(6, MechAnimation::TransType::LowerLegLeft, MechAnimation::TransType::FootLeft);

	// 右脚：上足〜膝、膝〜足
	SetCapsule(7, MechAnimation::TransType::UpperLegRight, MechAnimation::TransType::LowerLegRight);
	SetCapsule(8, MechAnimation::TransType::LowerLegRight, MechAnimation::TransType::FootRight);
}

void MechCollider::Draw() {
	for (auto& collider : colliders_) {
		MAGISYSTEM::DrawLineSphere(collider.p0, collider.radius, Color::Blue, 16);
		MAGISYSTEM::DrawLineSphere(collider.p1, collider.radius, Color::Blue, 16);
	}
}

const std::array<MechCollider::Capsule, 9>& MechCollider::GetList() const {
	return colliders_;
}

void MechCollider::SetCapsule(size_t idx, MechAnimation::TransType a, MechAnimation::TransType b) {
	// 端点ワールド座標
	colliders_[idx].p0 = mech_->GetPartsTransform(a)->GetWorldPosition();
	colliders_[idx].p1 = mech_->GetPartsTransform(b)->GetWorldPosition();

	// 半径
	colliders_[idx].radius = 0.5f;
}
