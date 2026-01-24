#include "Beam.h"

#include "MAGI.h"

using namespace Magi;

Beam::Beam(const Beam::InitParam& initParam, RefContext ref) :
	BaseProjectileAttack(initParam.initParam_, ref.damageCollisionSystem) {

	effectManager_ = ref.effectManager;

	// ひとまずの処理
	transform_->SetScale(Vector3(0.3f, 0.3f, 0.3f));
}

void Beam::Update() {
	// 基底クラスの更新
	BaseProjectileAttack::Update();

}

void Beam::Draw() {
	// ひとまず仮表示
	MAGISYSTEM::DrawSphere3D(transform_->GetWorldMatrix());

}

void Beam::Finalize() {
	BaseProjectileAttack::Finalize();
}
