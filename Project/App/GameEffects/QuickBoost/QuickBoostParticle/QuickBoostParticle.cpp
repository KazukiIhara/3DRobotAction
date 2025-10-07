#include "QuickBoostParticle.h"

#include "MAGI.h"
#include "MAGIAssert/MAGIAssert.h"

#include "GameObject/Mech/MechCore/MechCore.h"

#include "Random/Random.h"

QuickBoostParticle::QuickBoostParticle(MechCore* mechCore) {
	MAGIAssert::Assert(mechCore, "QuickBoostParticle Mechcore is null!");

	mech_ = mechCore;


	// 
	// 胴体パーティクル作成
	//
	bodySmokeEffect_.texIndex = MAGISYSTEM::GetTextureIndex("smoke.png");

	particleEffect_.texIndex = MAGISYSTEM::GetTextureIndex("Circle2.png");
	particleEffect_.size = { 0.02f,0.02f };

}

void QuickBoostParticle::Emit() {
	EmitBodyEffect();

}

void QuickBoostParticle::EmitBodyEffect() {
	if (auto body = mech_->GetMechBody()->GetGameObject().lock()) {
		// 胴体のワールド座標を取得
		const Vector3 emitPos = body->GetTransform()->GetWorldPosition();
		const Vector3 velocity = mech_->GetMovementComponent()->GetCurrentVelocity();

		for (uint32_t i = 0; i < smokeEffectNum_; i++) {
			bodySmokeEffect_.velo = Random::GenerateVector3(-2.0f, 2.0f);
			bodySmokeEffect_.pos = emitPos + Random::GenerateVector3(-1.0f, 1.0f);
			bodySmokeEffect_.life = Random::GenerateFloat(0.2f, 0.5f);

			MAGISYSTEM::EmitParticle(bodySmokeEffect_);
		}

		for (uint32_t i = 0; i < particleEffectNum_; i++) {
			particleEffect_.pos = emitPos;
			particleEffect_.velo = Random::GenerateVector3(-4.0f, 4.0f);

			particleEffect_.life = Random::GenerateFloat(0.2f, 0.4f);
			MAGISYSTEM::EmitParticle(particleEffect_);
		}
	}
}
