#include "Bullet.h"

#include "MAGI.h"
#include "GameObject/AttackCollider/AttackCollider.h"

Bullet::Bullet(const Vector3& dir, float speed, const Vector3& wPos, std::weak_ptr<AttackCollider> attackCollider) {
	BeginLife(baseLifeTime_, attackCollider);
	dir_ = dir;
	speed_ = speed;

	// トランスフォーム作成
	std::unique_ptr<Transform3D> trans = std::make_unique<Transform3D>(wPos);
	// トランスフォームマネージャに追加
	transform_ = MAGISYSTEM::AddTransform3D(std::move(trans));

	// パーティクルのデータ設定
	particleData_.size = { 0.02f,0.02f };
	particleData_.color = Color::Yellow;
	particleData_.texIndex = MAGISYSTEM::GetTextureIndex("white.png");

	lastEmitPos_ = transform_->GetTranslate();
}

void Bullet::Update() {
	const float dt = MAGISYSTEM::GetDeltaTime();
	if (!GetIsAlive()) {
		return;
	}

	if (CheckHitAndFinalize()) {
		return;
	}

	// 進行方向に向ける
	const Quaternion targetQ = DirectionToQuaternion(dir_);
	// 指定方向に移動
	const Vector3 velocity = dir_ * speed_ * dt;

	transform_->SetQuaternion(targetQ);
	transform_->AddTranslate(velocity);

	// コライダーにポジションをセット
	if (auto collider = LockCollider()) {
		collider->SetWorldPos(transform_->GetTranslate());
	}

	// パーティクル処理
	const Vector3 currPos = transform_->GetTranslate();
	const Vector3 a = lastEmitPos_;
	const Vector3 b = currPos;
	const Vector3 seg = b - a;
	const float segLen = Length(seg);

	emitAcc_ += emitRate_ * dt;
	int n = (int)std::floor(emitAcc_);
	emitAcc_ -= n;

	if (segLen > 1e-6f && n > 0) {
		for (int i = 0; i < n; ++i) {
			float u = (i + 0.5f) / (float)n;
			Vector3 p = Lerp(a, b, std::clamp(u, 0.0f, 1.0f));
			float dtOffset = (1.0f - u) * dt;

			particleData_.pos = p;
			particleData_.life = particleBaseLife_ - dtOffset;
			MAGISYSTEM::EmitParticle(particleData_);
		}
	}
	lastEmitPos_ = currPos;

	TickLifeAndFinalize(dt);
}

void Bullet::Draw() {
	MAGISYSTEM::DrawModel("Bullet", transform_->GetWorldMatrix(), material_);
}