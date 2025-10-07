#include "BulletHitParticle.h"

#include "MAGI.h"
#include "Math/Utility/MathUtility.h"

using namespace MAGIMath;

BulletHitParticle::BulletHitParticle(const Vector3& pos, const Vector3& velo, float gravity) {
	transform_ = std::make_unique<Transform3D>(pos);
	velocity_ = velo;
	gravity_ = gravity;
	mat_.blendMode = BlendMode::Normal;
}

void BulletHitParticle::Update() {

	// 生存時間の計算
	if (lifeTime_ > 0.0f) {
		lifeTime_ -= MAGISYSTEM::GetDeltaTime();
	} else {
		isAlive_ = false;
		return;
	}

	// 重力加速を追加
	velocity_.y += gravity_ * MAGISYSTEM::GetDeltaTime();

	// 移動量を加算
	transform_->AddTranslate(velocity_);

	// 移動方向を正規化
	const Vector3 dir = Normalize(velocity_);
	// 移動方向から回転を作成
	const Quaternion q = DirectionToQuaternion(dir);

	// 回転を適用
	transform_->SetQuaternion(q);

	// トランスフォーム更新
	transform_->Update();
}

void BulletHitParticle::Draw() {
	if (isAlive_) {
		MAGISYSTEM::DrawModel("Bullet", transform_->GetWorldMatrix(), mat_);
	}
}

bool BulletHitParticle::GetIsAlive()const {
	return isAlive_;
}