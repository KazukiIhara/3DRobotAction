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

}


void QuickBoostParticle::Emit() {
	EmitBodyEffect();

}

void QuickBoostParticle::Update() {
	if (auto body = mech_->GetMechBody()->GetGameObject().lock()) {
		// 胴体のワールド座標を取得
		const Vector3 emitPos = body->GetTransform()->GetWorldPosition();
		// パーティクルを発生
		for (uint32_t i = 0; i < 10; i++) {
			bodySmokeEffect_.velo = Random::GenerateVector3(-0.5f, 0.5f);
			bodySmokeEffect_.pos = emitPos + Random::GenerateVector3(-0.2f, 0.2f);
			bodySmokeEffect_.life = Random::GenerateFloat(0.5f, 0.8f);
			bodySmokeEffect_.size = Random::GenerateVector2(0.05f, 0.1f);

			MAGISYSTEM::EmitParticle(bodySmokeEffect_);
		}
	}
}

void QuickBoostParticle::EmitBodyEffect() {
	if (auto body = mech_->GetMechBody()->GetGameObject().lock()) {
		// 胴体のワールド座標を取得
		const Vector3 emitPos = body->GetTransform()->GetWorldPosition();
		// パーティクルを発生
		for (uint32_t i = 0; i < smokeEffectNum_; i++) {
			bodySmokeEffect_.velo = Random::GenerateVector3(-2.0f, 2.0f);
			bodySmokeEffect_.pos = emitPos + Random::GenerateVector3(-1.0f, 1.0f);
			bodySmokeEffect_.life = Random::GenerateFloat(0.2f, 0.5f);
			bodySmokeEffect_.size = Random::GenerateVector2(0.1f, 0.4f);

			MAGISYSTEM::EmitParticle(bodySmokeEffect_);
		}
	}
}
