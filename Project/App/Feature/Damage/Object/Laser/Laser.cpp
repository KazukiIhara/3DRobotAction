#define NOMINMAX

#include "Laser.h"

#include "Feature/Damage/CollisionSystem/DamageCollisionSystem.h"
#include "Feature/Effect/System/GameEffectManager/GameEffectManager.h"

#include "MAGI.h"

using namespace Magi;

Laser::Laser(const Laser::InitParam& initParam, RefContext ref) :
	BaseDamageObject(initParam.emitPos, ref.damageCollisionSystem) {

	// 初期化パラメータを受け取る
	emitPos_ = initParam.emitPos;
	dir_ = initParam.dir;
	speed_ = initParam.speed;
	life_ = initParam.life;

	effectManager_ = ref.effectManager;

	// コライダーを生成
	DamageCollider::Capsule capsule{
		.p0 = emitPos_,
		.p1 = transform_->GetWorldPosition(),
		.radius = 1.0f
	};
	AddDamageCollider(capsule, FriendlyTag::EnemySide);

	LaserEffect::InitParam eParam{};
	eParam.emitPos = emitPos_;
	eParam.dir = dir_;
	eParam.speed = speed_;
	eParam.life = life_;

	// レーザーエフェクト追加
	std::unique_ptr<LaserEffect> laserEffect = std::make_unique<LaserEffect>(eParam);
	effectManager_->Add(std::move(laserEffect));

}

void Laser::Update() {
	// ライフ更新
	life_ -= MAGISYSTEM::GetDeltaTime();
	life_ = std::max(0.0f, life_);
	if (life_ == 0.0f) {
		isAlive_ = false;
	}
	const Vector3 velocity = dir_ * speed_;
	transform_->AddTranslate(velocity);

}


void Laser::Draw() {
	// ひとまずエフェクトのみで描画処理なし

}

void Laser::Finalize() {
	BaseDamageObject::Finalize();

}
