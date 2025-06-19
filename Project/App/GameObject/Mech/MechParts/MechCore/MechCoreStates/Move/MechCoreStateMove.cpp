#include "MechCoreStateMove.h"

#include "GameObject/Mech/MechParts/MechCore/MechCore.h"

#include "MAGI.h"

using namespace MAGIMath;

void MechCoreStateMove::Enter(MechCore* mechCore) {
	// 速度リセット
	mechCore->GetMovementComponent()->SetMoveSpeed(0.0f);
}

void MechCoreStateMove::Update(MechCore* mechCore) {
	// コマンド取得
	InputCommand command = mechCore->GetInputCommand();

	// 移動入力なし　→　待機状態に遷移
	if (!Length(command.moveDirection)) {
		mechCore->ChangeState(MechCoreState::Idle);
	}

	// 移動量計算
	mechCore->GetMovementComponent()->Move(mechCore);
}

void MechCoreStateMove::Exit(MechCore* mechCore) {
	// 速度リセット
	mechCore->GetMovementComponent()->SetMoveSpeed(0.0f);
}
