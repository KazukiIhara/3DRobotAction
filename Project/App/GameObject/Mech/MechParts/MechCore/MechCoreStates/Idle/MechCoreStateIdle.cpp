#include "MechCoreStateIdle.h"

#include "GameObject/Mech/MechParts/MechCore/MechCore.h"

#include "Math/Utility/MathUtility.h"

using namespace MAGIMath;

void MechCoreStateIdle::Enter([[maybe_unused]] MechCore* mechCore) {

}

void MechCoreStateIdle::Update(MechCore* mechCore) {
	// コマンドを取得
	InputCommand command = mechCore->GetInputCommand();

	// 移動あり　→　移動状態に遷移
	if (Length(command.moveDirection)) {
		mechCore->ChangeState(MechCoreState::Move);
		return;
	}

	// 待機時の処理
	mechCore->GetMovementComponent()->Idle();
	// ジャンプの処理
	mechCore->GetMovementComponent()->Jump(mechCore);

	// 重力による移動量計算
	mechCore->GetMovementComponent()->CulGravityVelocity();
}

void MechCoreStateIdle::Exit(MechCore* mechCore) {
	// ジャンプの処理
	mechCore->GetMovementComponent()->Jump(mechCore);
	// 重力による移動量計算
	mechCore->GetMovementComponent()->CulGravityVelocity();
}
