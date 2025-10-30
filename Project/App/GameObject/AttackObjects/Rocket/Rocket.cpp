#include "Rocket.h"

#include "MAGI.h"

Rocket::Rocket(const Vector3& dir, float speed, const Vector3& wPos, std::weak_ptr<AttackCollider> attackCollider) {
	isAlive_ = true;
	lifeTime_ = 5.0f;
	dir_ = dir;
	speed_ = speed;

	ModelMaterial material{};
	material.blendMode = BlendMode::Add;

	// レンダラーを作成
	std::shared_ptr<ModelRenderer> rocketRenderer = std::make_shared<ModelRenderer>("Rocket", "Rocket", material);

	// ゲームオブジェクトを作成
	std::shared_ptr<GameObject3D> rocket = std::make_shared<GameObject3D>("Rocket", wPos);
	rocket->AddModelRenderer(rocketRenderer);
	rocket_ = MAGISYSTEM::AddGameObject3D(std::move(rocket));

	// 攻撃コライダーを設定
	collider_ = attackCollider;

}

void Rocket::Update() {
	// デルタタイム取得
	const float dt = MAGISYSTEM::GetDeltaTime();

	// ここで自分が持っているコライダーの衝突状況を取得できる
	// 自身の削除フラグを立てて衝突エフェクトの発火などをここで行ってもよいかも
	if (auto collider = collider_.lock()) {
		if (collider->GetHitInfo().isHit_) {
			// もし衝突してたらコライダーを消す
			collider->SetIsAlive(false);
			// 弾も消す
			Finalize();

			return;
		}
	}

	// 進行方向に向ける
	const Quaternion targetQ = DirectionToQuaternion(dir_);
	// 指定方向に移動
	const Vector3 velocity = dir_ * speed_ * dt;

	if (auto obj = rocket_.lock()) {
		obj->GetTransform()->SetQuaternion(targetQ);
		obj->GetTransform()->AddTranslate(velocity);

		// コライダーにポジションをセット	
		if (auto collider = collider_.lock()) {
			// ワールドポジションの場合まだ更新されていないためトランスレートをセット(親子付けしない前提)
			collider->SetWorldPos(obj->GetTransform()->GetTranslate());
		}

	}

	// 生存時間を減算
	lifeTime_ -= dt;
	if (lifeTime_ <= 0.0f) {
		Finalize();

		// コライダーを消す
		if (auto collider = collider_.lock()) {
			collider->SetIsAlive(false);
		}
	}


}

void Rocket::Draw() {

}

void Rocket::Finalize() {
	// 生存フラグをオフに
	isAlive_ = false;
	// オブジェクトを消す
	if (auto obj = rocket_.lock()) {
		obj->SetIsAlive(false);
	}
}

bool Rocket::GetIsAlive()const {
	return isAlive_;
}

AttackCollider* Rocket::GetAttackCollider() {
	return collider_.lock().get();
}

Vector3 Rocket::GetWorldPos() {
	Vector3 pos{};
	if (auto bullet = rocket_.lock()) {
		pos = bullet->GetTransform()->GetWorldPosition();
	}
	return pos;
}