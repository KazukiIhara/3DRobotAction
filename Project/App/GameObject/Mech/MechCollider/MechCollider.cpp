#include "MechCollider.h"

#include "MAGI.h"

MechCollider::MechCollider(const FriendlyTag& tag, const Vector3& posW, const Vector3& minL, const Vector3& maxL) {
	tag_ = tag;

	posW_ = posW;
	minL_ = minL;
	maxL_ = maxL;

	Update();
}

void MechCollider::Update() {
	// 衝突情報をリセット
	hitInfo_ = MechCollider::HitInfo{};
	minW_ = posW_ + minL_;
	maxW_ = posW_ + maxL_;
}

void MechCollider::Draw() {
	// デバッグ描画
	MAGISYSTEM::DrawLineAABB(minW_, maxW_, Color::Blue);
}

const Vector3& MechCollider::GetMinW() const {
	return minW_;
}

const Vector3& MechCollider::GetMaxW() const {
	return maxW_;
}

bool MechCollider::GetIsAlive()const {
	return isAlive_;
}

MechCollider::HitInfo MechCollider::GetHitInfo() const {
	return hitInfo_;
}

void MechCollider::SetWorldPos(const Vector3& posW) {
	posW_ = posW;
}

void MechCollider::SetIsAlive(bool isAlive) {
	isAlive_ = isAlive;
}

void MechCollider::SetHitInfo(HitInfo hitInfo) {
	hitInfo_ = hitInfo;
}
