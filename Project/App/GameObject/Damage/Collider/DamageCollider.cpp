#include "DamageCollider.h"

#include "MAGI.h"

using namespace Magi;

DamageCollider::DamageCollider(Param param) {
	param_ = std::move(param);

}

void DamageCollider::Update() {
	// 衝突情報をリセット
	hitInfo_ = DamageCollider::HitInfo{};
}

void DamageCollider::DebugDraw() {
	std::visit([&](const auto& shape) {
		using T = std::decay_t<decltype(shape)>;

		// Sphere
		if constexpr (std::is_same_v<T, Sphere>) {
			MAGISYSTEM::DrawLineSphere(shape.center, shape.radius, { 1,1,1,1 }, 16);
		}
		// OBB
		else if constexpr (std::is_same_v<T, OBB>) {
			// OBBのライン描画を呼ぶ
		}
		// Capsule
		else if constexpr (std::is_same_v<T, Capsule>) {
			// 端点球を描く
			MAGISYSTEM::DrawLineSphere(shape.p0, shape.radius, { 1,1,1,1 }, 16);
			MAGISYSTEM::DrawLineSphere(shape.p1, shape.radius, { 1,1,1,1 }, 16);

			// 側面の目安として線を引く（簡易）
			MAGISYSTEM::DrawLine3D(shape.p0, shape.p1, { 1,1,1,1 });
		}
		}, param_);
}

const DamageCollider::Param& DamageCollider::GetParam() const {
	return param_;
}

void DamageCollider::SetHitInfo(const HitInfo& hitInfo) {
	hitInfo_ = hitInfo;
}

DamageCollider::HitInfo DamageCollider::GetHitInfo() const {
	return hitInfo_;
}
void DamageCollider::SetIsAlive(bool isAlive) {
	isAlive_ = isAlive;
}

bool DamageCollider::GetIsAlive()const {
	return isAlive_;
}