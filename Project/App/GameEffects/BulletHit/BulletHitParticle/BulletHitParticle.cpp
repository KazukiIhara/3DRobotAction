#include "BulletHitParticle.h"


BulletHitParticle::BulletHitParticle(const Vector3& pos, const Vector3& velo) {
	
}

void BulletHitParticle::Update() {

	// トランスフォーム更新
	transform_.Update();
}

void BulletHitParticle::Draw() {


}