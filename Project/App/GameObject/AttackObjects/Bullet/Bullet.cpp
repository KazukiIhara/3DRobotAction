#include "Bullet.h"

#include "MAGI.h"
using namespace MAGI;

Bullet::Bullet(const Vector3& dir, float speed, const Vector3& wPos, std::weak_ptr<AttackCollider> attackCollider) {
	isAlive_ = true;
	lifeTime_ = 5.0f;
	dir_ = dir;
	speed_ = speed;

	ModelMaterial material{};
	material.blendMode = BlendMode::None;

	// レンダラーを作成
	std::shared_ptr<ModelRenderer> bulletRenderer = std::make_shared<ModelRenderer>("Bullet", "Bullet", material);

	// ゲームオブジェクトを作成
	std::shared_ptr<GameObject3D> bullet = std::make_shared<GameObject3D>("Bullet", wPos);
	bullet->AddModelRenderer(bulletRenderer);
	bullet_ = MAGISYSTEM::AddGameObject3D(std::move(bullet));

	// 攻撃コライダーを設定
	collider_ = attackCollider;

	// パーティクルのデータ設定
	particleData_.size = { 0.02f,0.02f };
	particleData_.color = Color::Yellow;
	particleData_.texIndex = MAGISYSTEM::GetTextureIndex("white.png");

	// トレイル用の設定
	if (auto obj = bullet_.lock()) {
		lastEmitPos_ = obj->GetTransform()->GetTranslate();
	}
}

void Bullet::Update() {
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

	if (auto obj = bullet_.lock()) {
		obj->GetTransform()->SetQuaternion(targetQ);
		obj->GetTransform()->AddTranslate(velocity);

		// コライダーにポジションをセット	
		if (auto collider = collider_.lock()) {
			// ワールドポジションの場合まだ更新されていないためトランスレートをセット(親子付けしない前提)
			collider->SetWorldPos(obj->GetTransform()->GetTranslate());
		}

		// パーティクル処理

		// 今フレームの弾の位置
		const Vector3 currPos = obj->GetTransform()->GetTranslate();
		// 今フレームの始点と終点
		const Vector3 a = lastEmitPos_;
		const Vector3 b = currPos;
		const Vector3 seg = b - a;
		const float segLen = Length(seg);

		// 今フレームに出す個数計算
		emitAcc_ += emitRate_ * dt;
		int n = (int)std::floor(emitAcc_);
		emitAcc_ -= n;

		// n個を線分内に均等配置
		if (segLen > 1e-6f && n > 0) {
			for (int i = 0; i < n; ++i) {
				// 中点サンプリング
				float u = (i + 0.5f) / (float)n;
				Vector3 p = Lerp(a, b, std::clamp(u, 0.0f, 1.0f));
				// その粒が既に経過している時間
				float dtOffset = (1.0f - u) * dt;

				// 座標と生存時間セット
				particleData_.pos = p;
				particleData_.life = particleBaseLife_ - dtOffset;
				// 発生
				MAGISYSTEM::EmitParticle(particleData_);
			}
		}
		lastEmitPos_ = currPos;

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

AttackCollider* Bullet::GetAttackCollider() {
	return collider_.lock().get();
}

Vector3 Bullet::GetWorldPos() {
	Vector3 pos{};
	if (auto bullet = bullet_.lock()) {
		pos = bullet->GetTransform()->GetWorldPosition();
	}
	return pos;
}
