#include "MechCoreStateMove.h"

#include "GameObject/Mech/MechParts/MechCore/MechCore.h"

#include "MAGI.h"

using namespace MAGIMath;

void MechCoreStateMove::Enter([[maybe_unused]] MechCore* mechCore) {

}

void MechCoreStateMove::Update(MechCore* mechCore) {
	// コマンド取得
	const InputCommand command = mechCore->GetInputCommand();

	// クイックブースト入力あり
	if (command.quickBoost) {
		mechCore->ChangeState(MechCoreState::QuickBoost);
		return;
	}

	// 移動入力なし　→　待機状態に遷移
	if (!Length(command.moveDirection)) {
		mechCore->ChangeState(MechCoreState::Idle);
		return;
	}

	// 移動量計算
	mechCore->GetMovementComponent()->Move(mechCore);
	// ジャンプの処理
	mechCore->GetMovementComponent()->Jump(mechCore);

	// 重力による移動量計算
	mechCore->GetMovementComponent()->CulGravityVelocity();

}

void MechCoreStateMove::Exit(MechCore* mechCore) {
	// ジャンプの処理
	mechCore->GetMovementComponent()->Jump(mechCore);
	// 重力による移動量計算
	mechCore->GetMovementComponent()->CulGravityVelocity();

}
