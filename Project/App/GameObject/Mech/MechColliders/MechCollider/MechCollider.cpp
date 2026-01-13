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
	hitInfos_.clear();
	minW_ = posW_ + minL_;
	maxW_ = posW_ + maxL_;
}

void MechCollider::Draw() {
	// デバッグ描画
#if defined(DEBUG) || defined(DEVELOP)
	MAGISYSTEM::DrawLineAABB(minW_, maxW_, Color::Blue);
#endif
}

const Vector3& MechCollider::GetMinW() const {
	return minW_;
}

const Vector3& MechCollider::GetMaxW() const {
	return maxW_;
}

void MechCollider::AddHitInfo(const HitInfo& info) {
	hitInfos_.push_back(info);
}

bool MechCollider::GetIsAlive()const {
	return isAlive_;
}

std::vector<MechCollider::HitInfo> MechCollider::GetHitInfo() const {
	return hitInfos_;
}

void MechCollider::SetWorldPos(const Vector3& posW) {
	posW_ = posW;
}

void MechCollider::SetIsAlive(bool isAlive) {
	isAlive_ = isAlive;
}