#include "BaseMechWeapon.h"

#include "MAGI.h"

using namespace Magi;

BaseMechWeapon::BaseMechWeapon(BaseMech* mech) {
	// 機体の参照ポインタを受け取る
	mech_ = mech;

	// 武器トランスフォームを作成
	std::unique_ptr<Transform3D> trans = std::make_unique<Transform3D>();
	transform_ = MAGISYSTEM::AddTransform3D(std::move(trans));

	// 攻撃発射場所トランスフォームを作成
	std::unique_ptr<Transform3D> fireTrans = std::make_unique<Transform3D>(
		Vector3(0.3f, 0.3f, 0.3f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f)
	);
	fireTransform_ = MAGISYSTEM::AddTransform3D(std::move(fireTrans));

	// 親子付け
	fireTransform_->SetParent(transform_, false);
}

Transform3D* BaseMechWeapon::GetTransform() {
	return transform_;
}

const Vector3 BaseMechWeapon::GetFireWorldPos() const {
	return fireTransform_->GetWorldPosition();
}

void BaseMechWeapon::DebugDraw() {
	MAGISYSTEM::DrawLineSphere(transform_->GetWorldPosition(), 0.5f, Color::Yellow);
	MAGISYSTEM::DrawLineSphere(fireTransform_->GetWorldPosition(), 0.5f, Color::Red);
}