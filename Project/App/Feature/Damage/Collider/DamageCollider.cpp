#include "DamageCollider.h"

#include "MAGI.h"

using namespace Magi;

DamageCollider::DamageCollider(Param param, const GameParam& gParam) {
	param_ = std::move(param);
	gParam_ = gParam;
}

void DamageCollider::Update() {

}

void DamageCollider::DebugDraw() {

	Vector4 color{};

	if (gParam_.tag == FriendlyTag::PlayerSide) {
		color = Color::Blue;
	} else {
		color = Color::Red;
	}

	std::visit([&](const auto& shape) {
		using T = std::decay_t<decltype(shape)>;

		// Sphere
		if constexpr (std::is_same_v<T, Sphere>) {
			MAGISYSTEM::DrawLineSphere(shape.center, shape.radius, color, 16);
		}
		// OBB
		else if constexpr (std::is_same_v<T, OBB>) {
			// OBBのライン描画を呼ぶ
		}
		// Capsule
		else if constexpr (std::is_same_v<T, Capsule>) {
			// 端点球を描く
			MAGISYSTEM::DrawLineSphere(shape.p0, shape.radius, color, 16);
			MAGISYSTEM::DrawLineSphere(shape.p1, shape.radius, color, 16);

			// 側面の目安として線を引く
			MAGISYSTEM::DrawLine3D(shape.p0, shape.p1, color);
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

DamageCollider::GameParam DamageCollider::GetGameParam() const {
	return gParam_;
}

Vector3 DamageCollider::GetCenter() {
	// 形状ごとに中心を返す
	return std::visit([](const auto& shape) -> Vector3 {

		// Sphere / OBB は center をそのまま返す
		// Capsule は端点の中点を中心とする
		using ShapeT = std::decay_t<decltype(shape)>;
		if constexpr (std::is_same_v<ShapeT, Capsule>) {
			return (shape.p0 + shape.p1) * 0.5f;
		} else {
			return shape.center;
		}

		}, param_);
}