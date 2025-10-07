#include "BulletHitEffect.h"

BulletHitEffect::BulletHitEffect() {

}

void BulletHitEffect::Emit(const Vector3& pos) {
	EmitParticle(pos);
}

void BulletHitEffect::Update() {

	// 全更新
	for (auto& p : bulletHitParticles_) {
		p.Update();
	}

	// 死亡フラグの立っているパーティクルを削除
	bulletHitParticles_.remove_if([](const BulletHitParticle& p) {
		return !p.GetIsAlive();
		});

}

void BulletHitEffect::Draw() {
	// 全描画
	for (auto& p : bulletHitParticles_) {
		p.Draw();
	}
}

void BulletHitEffect::EmitParticle(const Vector3& pos) {

}
