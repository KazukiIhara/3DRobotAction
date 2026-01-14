#include "Missile.h"

#include "MAGI.h"
#include "GameObject/Mech/MechCore/MechCore.h"
#include "GameObject/AttackCollider/AttackCollider.h"

Missile::Missile(
	const MissileType& missileType,
	const Vector3& wPos,
	const Vector3& dir,
	std::weak_ptr<MechCore> target,
	std::weak_ptr<AttackCollider> attackCollider
) {
	BeginLife(baseLifeTime_, attackCollider);
	dir_ = dir;

	type_ = missileType;

	switch (type_) {
	case MissileType::Dual:
		speed_ = 20.0f;
		boostAcc_ = 60.0f;
		maxBoostSpeed_ = 50.0f;
		boostTime_ = 0.5f;
		guidedAcc_ = 100.0f;
		maxGuidedSpeed_ = 80.0f;
		break;
	default:
		break;
	}

	// トランスフォーム作成
	std::unique_ptr<Transform3D> trans = std::make_unique<Transform3D>(wPos);
	// トランスフォームマネージャに追加
	transform_ = MAGISYSTEM::AddTransform3D(std::move(trans));

	// ターゲットを設定
	target_ = target;

	// フェーズを初期化
	phase_ = MissilePhase::Boost;
}

void Missile::Update() {
	const float dt = MAGISYSTEM::GetDeltaTime();

	if (!GetIsAlive()) {
		return;
	}

	if (CheckHitAndFinalize()) {
		return;
	}

	switch (type_) {
	case MissileType::Dual:
		UpdateDualMissile();
		break;
	default:
		break;
	}

	// 進行方向に向ける
	const Quaternion targetQ = DirectionToQuaternion(dir_);
	const Vector3 velocity = dir_ * speed_ * dt;

	transform_->SetQuaternion(targetQ);
	transform_->AddTranslate(velocity);

	// コライダーにポジションをセット
	if (auto collider = LockCollider()) {
		collider->SetWorldPos(transform_->GetTranslate());
	}

	TickLifeAndFinalize(dt);
}

void Missile::Draw() {
	MAGISYSTEM::DrawModel("Missile", transform_->GetWorldMatrix(), material_);
}

void Missile::EnterGuidedDualMissile() {
	if (auto targetObj = target_.lock()) {
		if (auto targetMechBodyObj = targetObj->GetMechBody()->GetGameObject().lock()) {
			const Vector3 targetPos = targetMechBodyObj->GetTransform()->GetWorldPosition();
			const Vector3 pos = transform_->GetWorldPosition();
			dir_ = Normalize(targetPos - pos);
		}
	}
}

void Missile::UpdateDualMissile() {
	const float dt = MAGISYSTEM::GetDeltaTime();

	switch (phase_) {
	case MissilePhase::Boost:
		speed_ += boostAcc_ * dt;
		boostTime_ -= dt;

		if (boostTime_ <= 0.0f) {
			phase_ = MissilePhase::Guided;
			EnterGuidedDualMissile();
		}
		speed_ = std::min(speed_, maxBoostSpeed_);
		break;

	case MissilePhase::Guided:
		speed_ += guidedAcc_ * dt;
		speed_ = std::min(speed_, maxGuidedSpeed_);
		break;
	}
}
