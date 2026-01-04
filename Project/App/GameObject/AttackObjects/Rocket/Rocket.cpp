#include "Rocket.h"

#include "MAGI.h"
#include "GameObject/AttackCollider/AttackCollider.h"

Rocket::Rocket(const Vector3& dir, float speed, const Vector3& wPos, std::weak_ptr<AttackCollider> attackCollider) {
	BeginLife(baseLifeTime_, attackCollider);
	dir_ = dir;
	speed_ = speed;

	// トランスフォーム作成
	std::unique_ptr<Transform3D> trans = std::make_unique<Transform3D>(wPos);

	// トランスフォームマネージャに追加
	transform_ = MAGISYSTEM::AddTransform3D(std::move(trans));

	// 球体データ
	sphereData_.radius = 0.2f;

	// マテリアル
	material_.textureName = "white.png";


}

void Rocket::Update() {
	// デルタタイム取得
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
		// ワールドポジションの場合まだ更新されていないためトランスレートをセット(親子付けしない前提)
		collider->SetWorldPos(transform_->GetTranslate());
	}

	TickLifeAndFinalize(dt);

}

void Rocket::Draw() {
	// 描画
	MAGISYSTEM::DrawSphere3D(transform_->GetWorldMatrix(), sphereData_, material_);
}

void Rocket::Finalize() {
	BaseAttackObject::Finalize();
}

Vector3 Rocket::GetWorldPos() {
	return transform_->GetWorldPosition();
}

void Rocket::OnFinalize() {
	transform_->SetIsAlive(false);
}