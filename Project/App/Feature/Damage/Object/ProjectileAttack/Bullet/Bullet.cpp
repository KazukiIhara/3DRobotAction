#include "Bullet.h"

#include "MAGI.h"

using namespace Magi;

Bullet::Bullet(const Bullet::InitParam& initParam, RefContext ref) :
	BaseProjectileAttack(initParam.initParam_, ref.damageCollisionSystem) {
	effectManager_ = ref.effectManager;

	// テクスチャのロード
	MAGISYSTEM::LoadTexture("YellowBullet.png");


	bulletTransParent_ = MAGISYSTEM::AddTransform3D();
	bulletTransParent_->SetParent(transform_, false);

	for (size_t i = 0; i < 2; i++) {
		bulletTrans_[i] = MAGISYSTEM::AddTransform3D();
		bulletTrans_[i]->SetParent(bulletTransParent_, false);
		bulletTrans_[i]->SetRotateY(-std::numbers::pi_v<float>*0.5f);
	}
	bulletTrans_[0]->SetRotateX(-std::numbers::pi_v<float>*0.25f);
	bulletTrans_[1]->SetRotateX(std::numbers::pi_v<float>*0.25f);

	bulletPlaneMat_.blendMode = BlendMode::Add;
	bulletPlaneMat_.textureName = "YellowBullet.png";

	effectScale_ = { 1.0f,1.0f,1.0f };
	effectMat_.baseColor = Color::Gold;
	effectMat_.blendMode = BlendMode::Add;
	effectMat_.textureName = "Circle.png";
}

void Bullet::Update() {
	// 基底クラスの更新
	BaseProjectileAttack::Update();

	const Vector3 dir = GetDir();
	const Quaternion q = DirectionToQuaternion_s(dir);
	bulletTransParent_->SetQuaternion(q);

	if (auto collider = GetDamageCollider()) {
		if (collider->GetHitInfo().isHit_) {
			// ヒット時エフェクトを出す


			// 生存フラグを消す
			isAlive_ = false;
		}
	}

}

void Bullet::Draw() {
	// 弾描画
	for (size_t i = 0; i < 2; i++) {
		MAGISYSTEM::DrawPlane3D(bulletTrans_[i]->GetWorldMatrix(), bulletPlane_[i], bulletPlaneMat_);
	}

	// 少しもわっとさせる
	const Vector3 trans = bulletTransParent_->GetWorldPosition();
	const Matrix4x4 wMat = MAGISYSTEM::GetCurrentCamera3D()->MakeBillBoardMat(trans, effectScale_);
	MAGISYSTEM::DrawPlane3D(wMat, PlaneData3D{}, effectMat_);
}

void Bullet::Finalize() {
	BaseProjectileAttack::Finalize();
}
