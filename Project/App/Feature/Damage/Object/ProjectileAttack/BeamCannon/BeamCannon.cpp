#include "BeamCannon.h"

#include "MAGI.h"

using namespace Magi;

BeamCannon::BeamCannon(const BeamCannon::InitParam& initParam, RefContext ref) :
	BaseProjectileAttack(initParam.initParam_, ref.damageCollisionSystem) {

	effectManager_ = ref.effectManager;

	// テクスチャのロード
	MAGISYSTEM::LoadTexture("Laser2.png");

	transParent_ = MAGISYSTEM::AddTransform3D();
	transParent_->SetParent(transform_, false);


	for (size_t i = 0; i < 4; i++) {
		trans_[i] = MAGISYSTEM::AddTransform3D(Vector3(0.0f, 0.0f, 0.0f));
		trans_[i]->SetParent(transParent_, false);
		trans_[i]->SetRotateY(-std::numbers::pi_v<float>*0.5f);
	}
	trans_[0]->SetRotateX(-std::numbers::pi_v<float>*0.25f);
	trans_[1]->SetRotateX(std::numbers::pi_v<float>*0.25f);

	trans_[2]->SetRotateX(-std::numbers::pi_v<float>*0.25f);
	trans_[3]->SetRotateX(std::numbers::pi_v<float>*0.25f);

	const float innerThickness = MAGISYSTEM::GetParameterValue<float>({ "DamageObjectParam","BeamCannon","InnerThickness" });
	const float outerThickness = MAGISYSTEM::GetParameterValue<float>({ "DamageObjectParam","BeamCannon","OuterThickness" });

	for (size_t i = 0; i < 2; i++) {
		trans_[i]->SetScale(Vector3(innerThickness, innerThickness, 1.0f));
	}
	for (size_t j = 2; j < 4; j++) {
		trans_[j]->SetScale(Vector3(outerThickness, outerThickness, 1.0f));
	}

	transParent_->SetScale(Vector3(1.0f, 1.0f, 0.1f));

	mat_.blendMode = BlendMode::Add;
	mat_.textureName = "Laser2.png";


	matBlue_.blendMode = BlendMode::Add;
	matBlue_.textureName = "Laser2.png";
	matBlue_.baseColor = Color::RoyalBlue;
}

void BeamCannon::Update() {
	// 基底クラスの更新
	BaseProjectileAttack::Update();

	const float length = GetLength();
	transParent_->SetScaleZ(length * 0.5f);

	const Vector3 dir = GetDir();
	const Quaternion q = DirectionToQuaternion_s(dir);
	transParent_->SetQuaternion(q);

	if (auto collider = GetDamageCollider()) {
		if (collider->GetHitInfo().isHit_) {
			// ヒット時エフェクトを出す


			// 生存フラグを消す
			isAlive_ = false;
		}
	}
}

void BeamCannon::Draw() {
	// ビーム描画
	// 弾描画
	for (size_t i = 0; i < 2; i++) {
		MAGISYSTEM::DrawPlane3D(trans_[i]->GetWorldMatrix(), PlaneData3D{}, mat_);
	}

	for (size_t j = 2; j < 4; j++) {
		MAGISYSTEM::DrawPlane3D(trans_[j]->GetWorldMatrix(), PlaneData3D{}, matBlue_);
	}
}

void BeamCannon::Finalize() {
	BaseProjectileAttack::Finalize();
	transParent_->SetIsAlive(false);

	for (size_t i = 0; i < 4; i++) {
		trans_[i]->SetIsAlive(false);
	}
}
