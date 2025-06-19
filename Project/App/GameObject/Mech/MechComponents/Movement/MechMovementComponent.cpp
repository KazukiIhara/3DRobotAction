#include "MechMovementComponent.h"

#include "GameObject/Mech/MechParts/MechCore/MechCore.h"

#include "MAGI.h"

using namespace MAGIMath;

void MechMovementComponent::Update(MechCore* mechCore) {

	// 計算済みの移動量を加算
	if (auto core = mechCore->GetGameObject().lock()) {
		core->GetTransform()->AddTranslate(velocity_ * MAGISYSTEM::GetDeltaTime());
	}
}

void MechMovementComponent::Idle(MechCore* mechCore) {

}

void MechMovementComponent::Move(MechCore* mechCore) {
	// コマンド取得
	InputCommand command = mechCore->GetInputCommand();

	// 加速度計算
	moveSpeed_ += kMoveAcc_ * MAGISYSTEM::GetDeltaTime();
	moveSpeed_ = std::min(moveSpeed_, kMaxMoveSpeed_);

	// 移動量計算
	velocity_.x = command.moveDirection.x * moveSpeed_;
	velocity_.z = command.moveDirection.y * moveSpeed_;

}

bool MechMovementComponent::CheckOnGround(MechCore* mechCore) {
	mechCore;
	return false;
}

void MechMovementComponent::SetMoveSpeed(float moveSpeed) {
	moveSpeed_ = moveSpeed;
}
