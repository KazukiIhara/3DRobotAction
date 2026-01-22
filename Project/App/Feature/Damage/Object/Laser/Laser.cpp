#define NOMINMAX

#include "Laser.h"

#include "Feature/Damage/CollisionSystem/DamageCollisionSystem.h"
#include "Feature/Effect/System/GameEffectManager/GameEffectManager.h"


#include "MAGI.h"

using namespace Magi;

Laser::Laser(const Laser::InitParam& initParam, RefContext ref) :
	BaseLinearAttack(initParam.initParam_, ref.damageCollisionSystem) {

	effectManager_ = ref.effectManager;

	// エフェクト作成
	Vector3 worldPos = transform_->GetWorldPosition();
	LaserEffect::InitParam eParam{};
	eParam.emitPos = worldPos;
	eParam.dir = dir_;
	eParam.life = life_;
	eParam.color;
	// レーザーエフェクト追加
	std::unique_ptr<LaserEffect> laserEffect = std::make_unique<LaserEffect>(eParam);
	laserEffect_ = dynamic_cast<LaserEffect*>(effectManager_->Add(std::move(laserEffect)));
	laserEffect_->SetLaserEndPos(endPos_);
}

void Laser::Update() {
	// 基底クラスのアップデート
	BaseLinearAttack::Update();
	// エフェクト更新
	laserEffect_->SetLaserEndPos(endPos_);

}


void Laser::Draw() {
	// ひとまずエフェクトのみで描画処理なし

}

void Laser::Finalize() {
	BaseLinearAttack::Finalize();
}
