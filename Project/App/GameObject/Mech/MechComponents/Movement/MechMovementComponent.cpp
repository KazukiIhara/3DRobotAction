#include "MechMovementComponent.h"

#include "GameObject/Mech/MechParts/MechCore/MechCore.h"

#include "MAGI.h"
#include "MAGIAssert/MAGIAssert.h"

using namespace MAGIMath;

void MechMovementComponent::Update(MechCore* mechCore) {
	// 計算済みの移動量を加算
	if (auto core = mechCore->GetGameObject().lock()) {
		core->GetTransform()->AddTranslate(velocity_ * MAGISYSTEM::GetDeltaTime());
	}
}

void MechMovementComponent::Idle() {
	// 接地状態　→　摩擦計算 
	if (onGround_) {
		velocity_.x *= kIdleFriction_;
		velocity_.z *= kIdleFriction_;
	}
}

void MechMovementComponent::Move(MechCore* mechCore) {
	// コマンド取得
	const InputCommand command = mechCore->GetInputCommand();

	// 加速度計算
	moveSpeed_ += kMoveAcc_ * MAGISYSTEM::GetDeltaTime();
	moveSpeed_ = std::min(moveSpeed_, kMaxMoveSpeed_);

	// 移動量に反映
	velocity_.x = command.moveDirection.x * moveSpeed_;
	velocity_.z = command.moveDirection.y * moveSpeed_;
}

void MechMovementComponent::Jump(MechCore* mechCore) {
	// コマンド取得
	const InputCommand command = mechCore->GetInputCommand();

	if (command.jump) {
		if (onGround_) { 	// 接地状態の場合
			// ジャンプの初速を与える
			velocity_.y = kJumpFirstSpeed_;
		} else {			// 空中にいる場合

		}
	}

}

void MechMovementComponent::CheckOnGround(MechCore* mechCore) {
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

void MechMovementComponent::CulGravityVelocity() {
	if (onGround_) {
		velocity_.y = 0.0f;
	} else {
		velocity_.y += kGravityAcc_ * MAGISYSTEM::GetDeltaTime();
	}
}

void MechMovementComponent::SetMoveSpeed(float moveSpeed) {
	moveSpeed_ = moveSpeed;
}
