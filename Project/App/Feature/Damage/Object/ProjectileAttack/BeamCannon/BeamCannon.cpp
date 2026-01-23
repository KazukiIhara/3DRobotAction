#include "BeamCannon.h"

#include "Feature/Damage/CollisionSystem/DamageCollisionSystem.h"

#include "MAGI.h"

using namespace Magi;

BeamCannon::BeamCannon(const BeamCannon::InitParam& initParam, RefContext ref) :
	BaseProjectileAttack(initParam.initParam_, ref.damageCollisionSystem) {

	effectManager_ = ref.effectManager;

	// エフェクトの作成など

}

void BeamCannon::Update() {
	// 基底クラスの更新
	BaseProjectileAttack::Update();

}

void BeamCannon::Draw() {
	MAGISYSTEM::DrawSphere3D(transform_->GetWorldMatrix());
}

void BeamCannon::Finalize() {
	BaseProjectileAttack::Finalize();
}
