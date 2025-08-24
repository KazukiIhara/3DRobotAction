#define NOMINMAX

#include "MechMovementComponent.h"

#include "GameObject/Mech/MechCore/MechCore.h"
#include "GameObject/PlayerCamera/PlayerCamera.h"

#include "MAGI.h"
#include "MAGIAssert/MAGIAssert.h"

using namespace MAGIMath;

MechMovementComponent::MechMovementComponent() {
	// クイックブーストのアニメーションを作成

}

void MechMovementComponent::Update(MechCore* mechCore) {
	// 現在のステートを取得
	const MechCoreState currentState = mechCore->GetCurrentState();

	// 接地している場合は縦の移動量の最低値を0にする
	if (onGround_) {
		velocity_.y = std::max(0.0f, velocity_.y);
	}

	// アサルトブースト時はすでに移動量を計算しているため計算しない
	if (currentState != MechCoreState::AssultBoost) {
		// スピードと向きからこのフレームでの移動量を計算
		velocity_.x = currentMoveDir_.x * moveSpeed_;
		velocity_.z = currentMoveDir_.y * moveSpeed_;
	}

	// 移動量をオブジェクトに加算
	if (auto core = mechCore->GetGameObject().lock()) {
		core->GetTransform()->AddTranslate(velocity_ * MAGISYSTEM::GetDeltaTime());
	}
}

void MechMovementComponent::Idle() {
	// 接地状態　→　摩擦計算 
	const float dt = MAGISYSTEM::GetDeltaTime();
	if (onGround_) {
		ApplyIdleFriction(moveSpeed_, kIdleFrictionGround_, dt);
	} else {
		ApplyIdleFriction(moveSpeed_, kIdleFrictionAir_, dt);
	}
}

void MechMovementComponent::Move(MechCore* mechCore) {
	// デルタタイム
	const float dt = MAGISYSTEM::GetDeltaTime();
	// コマンド取得
	const InputCommand command = mechCore->GetInputCommand();

	// 方向を設定
	currentMoveDir_ = command.moveDirection;

	// 加速度計算
	if (onGround_) {
		moveSpeed_ += kMoveAcc_ * dt;
	} else {
		moveSpeed_ += kMoveAccAir_ * dt;
	}

	moveSpeed_ = std::min(moveSpeed_, kMaxMoveSpeed_);
}

void MechMovementComponent::QuickBoostEnter(MechCore* mechCore) {
	// コマンド取得
	const InputCommand command = mechCore->GetInputCommand();

	// 移動方向をセット　
	currentMoveDir_ = command.moveDirection;

	// 速度をセット
	moveSpeed_ = kQuickBoostFirstSpeed_;

	// 降下速度をリセット
	velocity_.y = 0.0f;

	// タイマーセット
	quickBoostTimer_ = 0.0f;

	// カメラ揺れ
	const bool isHardLock = mechCore->GetLockOnComponent()->GetEnableHardLockOn();
	if (isHardLock) {
		MAGISYSTEM::ShakeCurrentCamera3D(kQuickBoostCameraShakeTime_, kQuickBoostCameraShakeIntensityHL_);
	} else {
		MAGISYSTEM::ShakeCurrentCamera3D(kQuickBoostCameraShakeTime_, kQuickBoostCameraShakeIntensitySL_);
	}
}

void MechMovementComponent::AssultBoostEnter(MechCore* mechCore) {
	// コマンド取得
	const InputCommand command = mechCore->GetInputCommand();

	// アサルトブースト時には使わないため0にする
	currentMoveDir_ = { 0.0f,0.0f };
	moveSpeed_ = 0.0f;
}

void MechMovementComponent::AssultBoostUpdate(MechCore* mechCore) {
	// コマンド取得
	const InputCommand command = mechCore->GetInputCommand();

	// カメラの方向を取得
	Vector3 cameraDir{};

	if (auto mechCoreObj = mechCore->GetGameObject().lock()) {
		if (auto camera = dynamic_cast<PlayerCamera*>(mechCoreObj->GetCamera3D("MainCamera").lock().get())) {
			const Quaternion localQ = camera->GetCameraQuaternion();
			const Quaternion bodyQ = mechCore->GetMechBody()->GetGameObject().lock()->GetTransform()->GetQuaternion();
			const Quaternion targetQ = Inverse(bodyQ) * localQ;

			cameraDir = Normalize(Transform(MakeForwardVector3(), localQ));

		}
	}

	// 速度の計算

	// カメラの方向に対して移動方向をセット
	velocity_ = cameraDir * kMaxAssultBoostSpeed_; // ひとまず最大値を入れておく

}

void MechMovementComponent::QuickBoostUpdate() {
	float t = std::clamp(quickBoostTimer_ / kQuickBoostTime_, 0.0f, 1.0f);
	moveSpeed_ = Lerp(moveSpeed_, kMaxMoveSpeed_, t);
	quickBoostTimer_ += MAGISYSTEM::GetDeltaTime();
}

bool MechMovementComponent::QuickBoostEndRequest() const {
	return quickBoostTimer_ > kQuickBoostTime_;
}

bool MechMovementComponent::QuickBoostEnableCancel() const {
	return quickBoostTimer_ > kQuickBoostCancelTime_;
}

void MechMovementComponent::ReverseInputDeceleration(MechCore* mechCore) {
	// コマンド取得
	const InputCommand command = mechCore->GetInputCommand();

	// 角度差を求める
	float dot = Dot(currentMoveDir_, command.moveDirection);
	if (dot < 0.0f) { // 90度以上差がある
		moveSpeed_ = 0.5f;
	}
}

void MechMovementComponent::Jump(MechCore* mechCore) {
	// コマンド取得
	const InputCommand command = mechCore->GetInputCommand();
	if (command.jump) {
		// 重力の影響を消す
		velocity_.y = std::max(velocity_.y, 0.0f);

		if (onGround_) { 	// 接地状態の場合
			// ジャンプの初速を与える
			velocity_.y = kJumpFirstSpeed_;
			// 空中判定にする
			onGround_ = false;
		} else {			// 空中にいる場合
			// 上昇加速度計算
			velocity_.y += kJumpAcc_ * MAGISYSTEM::GetDeltaTime();
		}
		// 上昇移動量をクランプ
		velocity_.y = std::min(velocity_.y, kMaxJumpSpeed_);
	}
}

void MechMovementComponent::CheckOnGround(MechCore* mechCore) {
	// ひとまず Y0.0f を地面とする
	if (auto it = mechCore->GetGameObject().lock()) {
		Transform3D* trans = it->GetTransform();
		if (trans->GetWorldPosition().y <= 0.0f) {
			onGround_ = true;
			trans->SetTranslateY(0.0f);
		} else {
			onGround_ = false;
		}

	} else {
		MAGIAssert::Assert(false, "PlayerObject Not found!");
	}
}

void MechMovementComponent::CulGravityVelocity(MechCore* mechCore) {
	// コマンド取得
	const InputCommand command = mechCore->GetInputCommand();

	if (!onGround_) {
		if (!command.jump) { // ジャンプ入力中は重力の影響を与えない
			velocity_.y += kGravityAcc_ * kGravityScale_ * MAGISYSTEM::GetDeltaTime();
		}
	}
}

Vector2 MechMovementComponent::GetCurrentMoveDir() const {
	return currentMoveDir_;
}

float MechMovementComponent::GetCurrentSpeed() const {
	return moveSpeed_;
}

float MechMovementComponent::GetMaxSpeed() const {
	return kMaxMoveSpeed_;
}

Vector3 MechMovementComponent::GetCurrentVelocity() const {
	return velocity_;
}

void MechMovementComponent::SetMoveSpeed(float s) {
	moveSpeed_ = s;
}

void MechMovementComponent::ApplyIdleFriction(float& v, float decelPerSec, float dt) {
	if (v > 0.0f) {
		// 正方向：v から減らす
		v = std::max(0.0f, v - decelPerSec * dt);
	} else if (v < 0.0f) {
		// 負方向：v に加える
		v = std::min(0.0f, v + decelPerSec * dt);
	}
	// v == 0 のときは何もしない
}