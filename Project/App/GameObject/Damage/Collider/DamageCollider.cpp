#include "DamageCollider.h"

DamageCollider::DamageCollider(Param param) {
	param_ = std::move(param);

}

void DamageCollider::Update() {
	// 衝突情報をリセット
	hitInfo_ = DamageCollider::HitInfo{};
}

void DamageCollider::DebugDraw() {

}

const DamageCollider::Param& DamageCollider::GetParam() const {
	return param_;
}

void DamageCollider::SetIsAlive(bool isAlive) {
	isAlive_ = isAlive;
}

bool DamageCollider::GetIsAlive()const {
	return isAlive_;
}