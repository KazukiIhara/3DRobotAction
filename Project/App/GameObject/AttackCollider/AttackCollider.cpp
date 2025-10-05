#include "AttackCollider.h"

#include "MAGI.h"

AttackCollider::AttackCollider(
	const FriendlyTag& tag,
	const AttackType& type,
	const Vector3& posW,
	const Vector3& minL,
	const Vector3& maxL,
	int32_t damage) {

	tag_ = tag;

	type_ = type;

	posW_ = posW;
	minL_ = minL;
	maxL_ = maxL;

	damage_ = damage;

	Update();
}

void AttackCollider::Update() {
	// 衝突情報をリセット
	hitInfo_ = AttackCollider::HitInfo{};

	minW_ = posW_ + minL_;
	maxW_ = posW_ + maxL_;
}

void AttackCollider::Draw() {
	// デバッグ描画
	switch (type_) {
		case AttackType::Bullet:
			MAGISYSTEM::DrawLineAABB(minW_, maxW_, Color::Crimson);
			break;
		case AttackType::Missile:
			MAGISYSTEM::DrawLineAABB(minW_, maxW_, Color::Yellow);
			break;
		case AttackType::Melee:
			break;
	}
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

AttackCollider::HitInfo AttackCollider::GetHitInfo() const {
	return hitInfo_;
}

AttackType AttackCollider::GetType()const {
	return type_;
}

int32_t AttackCollider::GetDamage() const {
	return damage_;
}

FriendlyTag AttackCollider::GetFriendlyTag()const {
	return tag_;
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
