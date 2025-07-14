#include "Bullet.h"

#include "MAGI.h"

Bullet::Bullet(const FriendlyTag& tag, const Vector3& dir, float speed, const Vector3& wPos) {
	tag_ = tag;
	isAlive_ = true;
	lifeTime_ = 10.0f;
	dir_ = dir;
	speed_ = speed;

	ModelMaterial material{};
	material.blendMode = BlendMode::Add;

	// レンダラーを作成
	std::shared_ptr<ModelRenderer> bulletRenderer = std::make_shared<ModelRenderer>("Bullet", "Bullet", material);

	// ゲームオブジェクトを作成
	std::shared_ptr<GameObject3D> bullet = std::make_shared<GameObject3D>("Bullet", wPos);
	bullet->AddModelRenderer(bulletRenderer);
	bullet_ = MAGISYSTEM::AddGameObject3D(std::move(bullet));

}

void Bullet::Update() {
	// 進行方向に向ける
	const Quaternion targetQ = DirectionToQuaternion(dir_);
	// 指定方向に移動
	const Vector3 velocity = dir_ * speed_ * MAGISYSTEM::GetDeltaTime();

	if (auto obj = bullet_.lock()) {
		obj->GetTransform()->SetQuaternion(targetQ);
		obj->GetTransform()->AddTranslate(velocity);
	}

	// 生存時間を減算
	lifeTime_ -= MAGISYSTEM::GetDeltaTime();
	if (lifeTime_ <= 0.0f) {
		isAlive_ = false;
		Finalize();
	}
}

void Bullet::Draw() {

}

void Bullet::Finalize() {
	// オブジェクトを消す
	if (auto obj = bullet_.lock()) {
		obj->SetIsAlive(false);
	}
}

bool Bullet::GetIsAlive()const {
	return isAlive_;
}