#include "AttackCollider.h"

#include "MAGI.h"

AttackCollider::AttackCollider(const Vector3& posW, const Vector3& minL, const Vector3& maxL) {
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

void AttackCollider::SetWorldPos(const Vector3& posW) {
	posW_ = posW;
}
