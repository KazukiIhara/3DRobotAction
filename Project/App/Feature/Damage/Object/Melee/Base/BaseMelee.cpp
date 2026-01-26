#include "BaseMelee.h"

#include "Feature/Damage/CollisionSystem/DamageCollisionSystem.h"

#include "MAGI.h"

using namespace Magi;

BaseMelee::BaseMelee(const BaseMelee::InitParam& initParam, DamageCollisionSystem* dmgColSystem) :
	BaseDamageObject(Vector3(0.0f, 0.0f, 0.0f), dmgColSystem) {

	// 初期化パラメータ受け取り
	parent_ = initParam.parent;
	radius_ = initParam.radius;
	life_ = initParam.life;

	// 親子付け
	transform_->SetParent(parent_, false);

	// コライダー作成
	const Vector3 worldPos = transform_->GetWorldPosition();
	DamageCollider::Sphere sphere{
		.center = worldPos,
		.radius = radius_
	};
	DamageCollider::GameParam gParam = initParam.gParam;
	AddDamageCollider(sphere, gParam);

}

void BaseMelee::Update() {
	const float dt = MAGISYSTEM::GetDeltaTime();

	// ライフ更新
	life_ -= dt;
	life_ = std::max(0.0f, life_);
	if (life_ == 0.0f) {
		isAlive_ = false;
		return;
	}

	// コライダー更新
	const Vector3 wPos = transform_->GetWorldPosition();
	DamageCollider::Sphere& param = GetDamageCollider()->GetParamMutableRef<DamageCollider::Sphere>();
	param.center = wPos;

}

void BaseMelee::Draw() {

}

void BaseMelee::Finalize() {
	BaseDamageObject::Finalize();
}

float BaseMelee::GetRadius() const {
	return radius_;
}

float BaseMelee::GetLife() const {
	return life_;
}