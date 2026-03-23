#include "BaseLinearAttack.h"

#include "Feature/Damage/CollisionSystem/DamageCollisionSystem.h"

#include "MAGI.h"

using namespace Magi;
using namespace MAGIMath;

BaseLinearAttack::BaseLinearAttack(
	const BaseLinearAttack::InitParam& initParam,
	DamageCollisionSystem* damageCollisionSystem
) :
	BaseDamageObject(initParam.emitPos, damageCollisionSystem) {

	// 初期化パラメータを受け取る
	dir_ = Normalize(initParam.dir);
	speed_ = initParam.speed;
	life_ = initParam.life;
	// 少し前方を終点にしておく
	endPos_ = initParam.emitPos + dir_;

	// コライダーを生成
	const Vector3 worldPos = transform_->GetWorldPosition();
	DamageCollider::Capsule capsule{
		.p0 = worldPos,
		.p1 = endPos_,
		.radius = 1.0f
	};
	DamageCollider::GameParam gParam = initParam.gParam;
	AddDamageCollider(capsule, gParam);

}


void BaseLinearAttack::Update() {
	const float dt = MAGISYSTEM::GetDeltaTime();
	// ライフ更新
	life_ -= dt;
	life_ = std::max(0.0f, life_);
	if (life_ == 0.0f) {
		isAlive_ = false;
		return;
	}
	const Vector3 velocity = dir_ * speed_;
	endPos_ += velocity * dt;

	// コライダー更新
	DamageCollider::Capsule& param = GetDamageCollider()->GetParamMutableRef<DamageCollider::Capsule>();
	param.p1 = endPos_;

}

void BaseLinearAttack::Draw() {

}

void BaseLinearAttack::Finalize() {
	BaseDamageObject::Finalize();
}
