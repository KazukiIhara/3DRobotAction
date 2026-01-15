#include "BossMechBaseWeapon.h"

#include "MAGI.h"

#include "GameObject/AttackObjectManager/AttackObjectManager.h"
#include "GameEffects/System/GameEffectManager/GameEffectManager.h"

using namespace Magi;

BossMechBaseWeapon::BossMechBaseWeapon(BossMech* mech) {
	// 機体の参照ポインタを受け取る
	mech_ = mech;

	// 武器トランスフォームを作成
	std::unique_ptr<Transform3D> trans = std::make_unique<Transform3D>();
	transform_ = MAGISYSTEM::AddTransform3D(std::move(trans));

	// 攻撃発射場所トランスフォームを作成
	std::unique_ptr<Transform3D> fireTrans = std::make_unique<Transform3D>();
	fireTransform_ = MAGISYSTEM::AddTransform3D(std::move(fireTrans));

	// 親子付け
	fireTransform_->SetParent(transform_);
}

Transform3D* BossMechBaseWeapon::GetTransform() {
	return transform_;
}

const Vector3 BossMechBaseWeapon::GetFireWorldPos() const {
	return fireTransform_->GetWorldPosition();
}
