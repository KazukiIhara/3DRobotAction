#include "BaseProjectileAttack.h"

#include "Feature/Damage/CollisionSystem/DamageCollisionSystem.h"

#include "MAGI.h"

using namespace Magi;

BaseProjectileAttack::BaseProjectileAttack(
	const BaseProjectileAttack::InitParam& initParam,
	DamageCollisionSystem* damageCollisionSys
) :
	BaseDamageObject(initParam.emitPos, damageCollisionSys) {

	// 初期化パラメータを受け取る
	dir_ = Normalize(initParam.dir);
	thickness_ = initParam.thickness;
	length_ = initParam.length;
	speed_ = initParam.speed;
	life_ = initParam.life;


	// コライダーを生成
	const Vector3 worldPos = transform_->GetWorldPosition();
	DamageCollider::Capsule capsule{
		.p0 = worldPos,
		.p1 = worldPos - dir_ * length_,
		.radius = thickness_
	};
	DamageCollider::GameParam gParam = initParam.gParam;
	AddDamageCollider(capsule, gParam);

}

void BaseProjectileAttack::Update() {
	const float dt = MAGISYSTEM::GetDeltaTime();

	// ライフ更新
	life_ -= dt;
	life_ = std::max(0.0f, life_);
	if (life_ == 0.0f) {
		isAlive_ = false;
		return;
	}

	// 弾を移動
	const Vector3 velocity = dir_ * speed_;
	transform_->AddTranslate(velocity * dt);

	// コライダー更新
	const Vector3 wPos = transform_->GetWorldPosition();
	DamageCollider::Capsule& param = GetDamageCollider()->GetParamMutableRef<DamageCollider::Capsule>();
	param.p0 = wPos;
	param.p1 = wPos - dir_ * length_;

}

void BaseProjectileAttack::Draw() {

}

void BaseProjectileAttack::Finalize() {
	BaseDamageObject::Finalize();
}

Vector3 BaseProjectileAttack::GetDir() const {
	return dir_;
}

float BaseProjectileAttack::GetThickness() const {
	return thickness_;
}

float BaseProjectileAttack::GetLength() const {
	return length_;
}

float BaseProjectileAttack::GetSpeed() const {
	return speed_;
}

float BaseProjectileAttack::GetLife() const {
	return life_;
}