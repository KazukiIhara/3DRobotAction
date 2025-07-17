#include "AttackCollider.h"

#include "MAGI.h"

AttackCollider::AttackCollider(const FriendlyTag& tag, const Vector3& posW, const Vector3& minL, const Vector3& maxL) {
	tag_ = tag;

	posW_ = posW;
	minL_ = minL;
	maxL_ = maxL;

	Update();
}

void AttackCollider::Update() {
	minW_ = posW_ + minL_;
	maxW_ = posW_ + maxL_;
}

void AttackCollider::Draw() {
	// デバッグ描画
	MAGISYSTEM::DrawLineAABB(minW_, maxW_, Color::Crimson);
}

const Vector3& AttackCollider::GetMinW() const {
	return minW_;
}

const Vector3& AttackCollider::GetMaxW() const {
	return maxW_;
}

bool AttackCollider::GetIsAlive()const {
	return isAlive_;
}

HitInfo AttackCollider::GetHitInfo() const {
	return hitInfo_;
}

void AttackCollider::SetWorldPos(const Vector3& posW) {
	posW_ = posW;
}

void AttackCollider::SetIsAlive(bool isAlive) {
	isAlive_ = isAlive;
}

void AttackCollider::SetHitInfo(HitInfo hitInfo) {
	hitInfo_ = hitInfo;
}
