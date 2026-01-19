#include "MechCollider.h"

#include "MAGI.h"

MechaCollider::MechaCollider(const FriendlyTag& tag, const Vector3& posW, const Vector3& minL, const Vector3& maxL) {
	tag_ = tag;

	posW_ = posW;
	minL_ = minL;
	maxL_ = maxL;

	Update();
}

void MechaCollider::Update() {
	// 衝突情報をリセット
	hitInfos_.clear();
	minW_ = posW_ + minL_;
	maxW_ = posW_ + maxL_;
}

void MechaCollider::Draw() {
	// デバッグ描画
#if defined(DEBUG) || defined(DEVELOP)
	MAGISYSTEM::DrawLineAABB(minW_, maxW_, Color::Blue);
#endif
}

const Vector3& MechaCollider::GetMinW() const {
	return minW_;
}

const Vector3& MechaCollider::GetMaxW() const {
	return maxW_;
}

void MechaCollider::AddHitInfo(const HitInfo& info) {
	hitInfos_.push_back(info);
}

bool MechaCollider::GetIsAlive()const {
	return isAlive_;
}

std::vector<MechaCollider::HitInfo> MechaCollider::GetHitInfo() const {
	return hitInfos_;
}

void MechaCollider::SetWorldPos(const Vector3& posW) {
	posW_ = posW;
}

void MechaCollider::SetIsAlive(bool isAlive) {
	isAlive_ = isAlive;
}