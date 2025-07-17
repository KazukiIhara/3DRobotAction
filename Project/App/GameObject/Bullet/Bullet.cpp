#include "Bullet.h"

#include "MAGI.h"

#include "GameObject/AttackCollider/AttackCollider.h"

Bullet::Bullet(const Vector3& dir, float speed, const Vector3& wPos, std::weak_ptr<AttackCollider> attackCollider) {
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

	// 攻撃コライダーを設定
	collider_ = attackCollider;
}

void Bullet::Update() {
	// ここで自分が持っているコライダーの衝突状況を取得できる
	// 自身の削除フラグを立てて衝突エフェクトの発火などをここで行ってもよいかも
	if (auto collider = collider_.lock()) {
		collider->GetHitInfo().isHit_;
	}

	// 進行方向に向ける
	const Quaternion targetQ = DirectionToQuaternion(dir_);
	// 指定方向に移動
	const Vector3 velocity = dir_ * speed_ * MAGISYSTEM::GetDeltaTime();

	if (auto obj = bullet_.lock()) {
		obj->GetTransform()->SetQuaternion(targetQ);
		obj->GetTransform()->AddTranslate(velocity);

		// コライダーにポジションをセット	
		if (auto collider = collider_.lock()) {
			// ワールドポジションの場合まだ更新されていないためトランスレートをセット(親子付けしない前提)
			collider->SetWorldPos(obj->GetTransform()->GetTranslate());
		}

	}

	// 生存時間を減算
	lifeTime_ -= MAGISYSTEM::GetDeltaTime();
	if (lifeTime_ <= 0.0f) {
		Finalize();

		// コライダーを消す
		if (auto collider = collider_.lock()) {
			collider->SetIsAlive(false);
		}
	}

}

void Bullet::Draw() {

}

void Bullet::Finalize() {
	// 生存フラグをオフに
	isAlive_ = false;
	// オブジェクトを消す
	if (auto obj = bullet_.lock()) {
		obj->SetIsAlive(false);
	}
}

bool Bullet::GetIsAlive()const {
	return isAlive_;
}