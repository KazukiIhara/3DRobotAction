#include "Bullet.h"

#include "MAGI.h"

using namespace Magi;

Bullet::Bullet(const Bullet::InitParam& initParam, RefContext ref) :
	BaseProjectileAttack(initParam.initParam_, ref.damageCollisionSystem) {

	effectManager_ = ref.effectManager;

	// ひとまずの処理
	transform_->SetScale(Vector3(0.3f, 0.3f, 0.3f));
}

void Bullet::Update() {
	// 基底クラスの更新
	BaseProjectileAttack::Update();


	if (auto collider = GetDamageCollider()) {
		if (collider->GetHitInfo().isHit_) {
			// ヒット時エフェクトを出す


			// 生存フラグを消す
			isAlive_ = false;
		}
	}

}

void Bullet::Draw() {
	// ひとまず仮表示
	MAGISYSTEM::DrawSphere3D(transform_->GetWorldMatrix());

}

void Bullet::Finalize() {
	BaseProjectileAttack::Finalize();
}
