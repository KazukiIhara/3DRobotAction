#include "LaserBladeSlash.h"

LaserBladeSlash::LaserBladeSlash(const LaserBladeSlash::InitParam& initParam, RefContext ref) :
	BaseMelee(initParam.initParam_, ref.damageCollisionSystem) {
	effectManager_ = ref.effectManager;

	// エフェクトの作成など

}

void LaserBladeSlash::Update() {
	BaseMelee::Update();

	if (auto collider = GetDamageCollider()) {
		if (collider->GetHitInfo().isHit_) {
			// ヒット時エフェクトを出す


			// 生存フラグを消す
			isAlive_ = false;
		}
	}


}

void LaserBladeSlash::Draw() {

}

void LaserBladeSlash::Finalize() {
	BaseMelee::Finalize();
}
