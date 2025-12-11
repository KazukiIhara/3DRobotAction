#include "BulletHitEffect.h"

#include "MAGI.h"
#include "Random/Random.h"

#include "GameObject/Mech/MechCore/MechCore.h"

BulletHitEffect::BulletHitEffect(MechCore* mechCore) {
	mech_ = mechCore;
}

void BulletHitEffect::Emit(const Vector3& pos) {
	EmitParticle(pos);
}

void BulletHitEffect::Update() {
	// 全更新
	for (auto& p : bulletHitParticles_) {
		p->Update();
	}

	// 死亡フラグの立っているパーティクルを削除
	bulletHitParticles_.remove_if([](const std::unique_ptr<BulletHitParticle>& p) {
		return !p->GetIsAlive();
		});

#if defined(DEBUG) || defined(DEVELOP)
	if (mech_->GetFriendlyTag() == FriendlyTag::EnemySide) {
		ImGui::Begin("BulletHitEffect");
		ImGui::DragFloat("DiffusionRange", &diffusionRange_, 0.01f);
		ImGui::DragFloat("Gravity", &gravity_, 0.01f);
		ImGui::End();
	}
#endif // _DEBUG

}

void BulletHitEffect::Draw() {
	// 全描画
	for (auto& p : bulletHitParticles_) {
		p->Draw();
	}
}

void BulletHitEffect::EmitParticle(const Vector3& pos) {
	// パーティクル追加処理
	for (uint32_t i = 0; i < emitNum_; i++) {
		const Vector3 randVelo = Random::GenerateVector3(-diffusionRange_, diffusionRange_);
		std::unique_ptr<BulletHitParticle> newP = std::make_unique<BulletHitParticle>(pos, randVelo, gravity_);
		bulletHitParticles_.push_back(std::move(newP));
	}
}
