#include "Missile.h"

#include "MAGI.h"

#include "GameObject/AttackCollider/AttackCollider.h"

Missile::Missile(const MissileType& missileType, const Vector3& wPos, float speed, const Vector3& dir, std::weak_ptr<MechCore> target, std::weak_ptr<AttackCollider> attackCollider) {
	isAlive_ = true;
	lifeTime_ = 10.0f;
	dir_ = dir;
	speed_ = speed;
	type_ = missileType;

	// レンダラーを作成
	std::shared_ptr<ModelRenderer> renderer = std::make_shared<ModelRenderer>("Missile", "MechRightArm");

	// ゲームオブジェクトを作成
	std::shared_ptr<GameObject3D> missile = std::make_shared<GameObject3D>("Missile", wPos);
	missile->AddModelRenderer(renderer);
	missile_ = MAGISYSTEM::AddGameObject3D(std::move(missile));

	// ターゲットを設定
	target_ = target;

	// 攻撃コライダーを設定
	collider_ = attackCollider;

	// フェーズを初期化
	phase_ = MissilePhase::Boost;
}

void Missile::Update() {
	// 衝突判定
	if (auto collider = collider_.lock()) {
		if (collider->GetHitInfo().isHit_) {
			// もし衝突してたらコライダーを消す
			collider->SetIsAlive(false);
			// 弾も消す
			Finalize();

			return;
		}
	}

	// ここにミサイル固有の更新処理を実装する
	switch (type_) {
		case MissileType::Dual:
			UpdateDualMissile();
			break;
		default:
			break;
	}

	// 進行方向に向ける
	const Quaternion targetQ = DirectionToQuaternion(dir_);
	// 指定方向に移動
	const Vector3 velocity = dir_ * speed_ * MAGISYSTEM::GetDeltaTime();

	if (auto obj = missile_.lock()) {
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

void Missile::Finalize() {
	// 生存フラグをオフに
	isAlive_ = false;
	// オブジェクトを消す
	if (auto obj = missile_.lock()) {
		obj->SetIsAlive(false);
	}
}

bool Missile::GetIsAlive()const {
	return isAlive_;
}

AttackCollider* Missile::GetAttackCollider() {
	return collider_.lock().get();
}

Vector3 Missile::GetWorldPos() {
	Vector3 pos{};
	if (auto bullet = missile_.lock()) {
		pos = bullet->GetTransform()->GetWorldPosition();
	}
	return pos;
}

void Missile::UpdateDualMissile() {
	switch (phase_) {
		case MissilePhase::Boost:

			break;
		case MissilePhase::Guided:

			break;
		default:
			break;
	}
}