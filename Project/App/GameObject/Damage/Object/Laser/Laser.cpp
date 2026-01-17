#include "Laser.h"

#include "GameObject/Damage/CollisionSystem/DamageCollisionSystem.h"

#include "MAGI.h"

using namespace Magi;

Laser::Laser(const Laser::InitParam& initParam) :
	BaseDamageObject(initParam.emitPos) {
	// 初期化パラメータを受け取る
	dir_ = initParam.dir;
	speed_ = initParam.speed;
	life_ = initParam.life;
	// コライダーを生成
	
}

void Laser::Update() {
	// ライフ更新
	life_ -= MAGISYSTEM::GetDeltaTime();
	if (std::max(0.0f, life_) == 0.0f) {
		isAlive_ = false;
	}
	const Vector3 velocity = dir_ * speed_;
	transform_->AddTranslate(velocity);
}


void Laser::Draw() {
	// ひとまずエフェクトのみで描画処理なし

}