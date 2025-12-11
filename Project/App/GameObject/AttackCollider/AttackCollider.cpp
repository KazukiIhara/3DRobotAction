#include "AttackCollider.h"

#include "MAGI.h"
using namespace MAGI;

AttackCollider::AttackCollider(
	const FriendlyTag& tag,
	const AttackType& type,
	const Vector3& posW,
	float radius,
	int32_t damage) {

	tag_ = tag;
	type_ = type;
	posW_ = posW;
	radius_ = radius;
	damage_ = damage;

	Update();
}

void AttackCollider::Update() {
	// 衝突情報をリセット
	hitInfo_ = AttackCollider::HitInfo{};
}

void AttackCollider::Draw() {
	// デバッグ描画
	switch (type_) {
	case AttackType::Bullet:
	case AttackType::Rocket:
	case AttackType::Missile:
		MAGISYSTEM::DrawLineSphere(posW_, radius_, Color::Crimson);
		break;
	case AttackType::Melee:
		break;
	}
}

const Vector3& AttackCollider::GetWPos() const {
	return posW_;
}

float AttackCollider::GetRadius() const {
	return radius_;
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
