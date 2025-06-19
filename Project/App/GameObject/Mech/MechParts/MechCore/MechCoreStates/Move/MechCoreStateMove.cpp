#include "MechCoreStateMove.h"

#include "GameObject/Mech/MechParts/MechCore/MechCore.h"

#include "MAGI.h"

using namespace MAGIMath;

void MechCoreStateMove::Enter([[maybe_unused]] MechCore* mechCore) {
	// 速度リセット
	moveSpeed_ = 0.0f;
}

void MechCoreStateMove::Update(MechCore* mechCore) {
	// コマンド取得
	InputCommand command = mechCore->GetInputCommand();

	// 移動入力なし　→　移動状態に遷移
	if (!Length(command.moveDirection)) {
		mechCore->ChangeState(MechCoreState::Move);
	}

	// 加速
	moveSpeed_ += moveAcc_ * MAGISYSTEM::GetDeltaTime();

	// 移動量計算
	Vector3 velocity = { command.moveDirection.x, 0.0f, command.moveDirection.y };
	velocity *= moveSpeed_;

	// 移動量セット
	mechCore->SetVelocity(velocity);
}

void MechCoreStateMove::Exit([[maybe_unused]] MechCore* mechCore) {
	// 速度リセット
	moveSpeed_ = 0.0f;
}
