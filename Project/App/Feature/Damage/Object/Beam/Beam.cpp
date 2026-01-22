#define NOMINMAX

#include "Beam.h"

#include "Feature/Damage/CollisionSystem/DamageCollisionSystem.h"
#include "Feature/Effect/System/GameEffectManager/GameEffectManager.h"

#include "MAGI.h"

using namespace Magi;

Beam::Beam(const Beam::InitParam& initParam, RefContext ref) :
	BaseDamageObject(initParam.emitPos, ref.damageCollisionSystem) {

	// 初期化パラメータを受け取る
	dir_ = Normalize(initParam.dir);
	speed_ = initParam.speed;
	life_ = initParam.life;
	beamEndPos_ = initParam.emitPos + dir_;
	thickness_ = initParam.thickness;
	effectManager_ = ref.effectManager;

	const Vector3 worldPos = transform_->GetWorldPosition();
	// コライダーを作成
	DamageCollider::Capsule capsule{
		.p0 = worldPos,
		.p1 = beamEndPos_,
		.radius = 1.0f
	};
	AddDamageCollider(capsule, initParam.tag);


	// エフェクト作成、追加

}

void Beam::Update() {

}

void Beam::Draw() {

}

void Beam::Finalize() {
	BaseDamageObject::Finalize();
}
