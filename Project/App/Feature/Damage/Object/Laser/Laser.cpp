#define NOMINMAX

#include "Laser.h"

#include "Feature/Damage/CollisionSystem/DamageCollisionSystem.h"
#include "Feature/Effect/System/GameEffectManager/GameEffectManager.h"

#include "MAGI.h"

using namespace Magi;

Laser::Laser(const Laser::InitParam& initParam, RefContext ref) :
	BaseDamageObject(initParam.emitPos, ref.damageCollisionSystem) {

	// 初期化パラメータを受け取る
	dir_ = Normalize(initParam.dir);
	speed_ = initParam.speed;
	life_ = initParam.life;
	laserEndPos_ = initParam.emitPos + dir_;
	effectManager_ = ref.effectManager;

	const Vector3 worldPos = transform_->GetWorldPosition();
	// コライダーを生成
	DamageCollider::Capsule capsule{
		.p0 = worldPos,
		.p1 = laserEndPos_,
		.radius = 1.0f
	};
	AddDamageCollider(capsule, FriendlyTag::EnemySide);

	// エフェクト作成
	LaserEffect::InitParam eParam{};
	eParam.emitPos = worldPos;
	eParam.dir = dir_;
	eParam.life = life_;

	// レーザーエフェクト追加
	std::unique_ptr<LaserEffect> laserEffect = std::make_unique<LaserEffect>(eParam);
	laserEffect_ = dynamic_cast<LaserEffect*>(effectManager_->Add(std::move(laserEffect)));
	laserEffect_->SetLaserEndPos(laserEndPos_);
}

void Laser::Update() {

	const float dt = MAGISYSTEM::GetDeltaTime();

	// ライフ更新
	life_ -= dt;
	life_ = std::max(0.0f, life_);
	if (life_ == 0.0f) {
		isAlive_ = false;
		return;
	}
	const Vector3 velocity = dir_ * speed_;
	laserEndPos_ += velocity * dt;

	// コライダー更新
	DamageCollider::Capsule& param = GetDamageCollider()->GetParamMutableRef<DamageCollider::Capsule>();
	param.p1 = laserEndPos_;

	// エフェクト更新
	laserEffect_->SetLaserEndPos(laserEndPos_);

}


void Laser::Draw() {
	// ひとまずエフェクトのみで描画処理なし

}

void Laser::Finalize() {
	BaseDamageObject::Finalize();

}
