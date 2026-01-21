#include "MechJustDodgeCollider.h"

#include "GameObject/Mech/MechCore/MechCore.h"

#include "MAGI.h"

using namespace Magi;

MechJustDodgeCollider::MechJustDodgeCollider() {
	// パラメータを作成
	MAGISYSTEM::AddParameterData({ "MechCommonParam","JustDodge","ColliderRadius" }, ParamType::Float);

}

void MechJustDodgeCollider::Update(MechCore* core) {
	// 衝突情報リセット
	isHit_ = false;

	// コライダーのワールド座標を更新
	const Vector3 bodyWorldPos = core->GetMechBody()->GetGameObject().lock()->GetTransform()->GetWorldPosition();
	worldPos_ = bodyWorldPos;

	// コライダーのサイズを更新
	radius_ = MAGISYSTEM::GetParameterValue<float>({ "MechCommonParam","JustDodge","ColliderRadius" });

}

void MechJustDodgeCollider::Draw() const {
	// デバッグ描画
#if defined (DEBUG)|(DEVELOP)
	MAGISYSTEM::DrawLineSphere(worldPos_, radius_, Color::Blue);
#endif
}

Vector3 MechJustDodgeCollider::GetWorldPos() const {
	return worldPos_;
}

float MechJustDodgeCollider::GetRadius() const {
	return radius_;
}

const bool MechJustDodgeCollider::GetIsHit() const {
	return isHit_;
}

void MechJustDodgeCollider::SetIsHit(bool isHit) {
	isHit_ = isHit;
}