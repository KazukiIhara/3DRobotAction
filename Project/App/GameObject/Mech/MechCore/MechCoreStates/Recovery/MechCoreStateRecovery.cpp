#include "MechCoreStateRecovery.h"

#include "GameObject/Mech/MechCore/MechCore.h"

void MechCoreStateRecovery::Enter(MechCore* mechCore) {
	// 移動速度を0にする
	mechCore->GetMovementComponent()->ResetVelocity();
}

void MechCoreStateRecovery::Update(MechCore* mechCore) {
	// 硬直終了、Idle状態に遷移
	if (mechCore->GetStatusComponent()->GetRecoveryTime() == 0.0f) {
		mechCore->ChangeState(MechCoreState::Idle);
	}
}

void MechCoreStateRecovery::Exit([[maybe_unused]]MechCore* mechCore) {

}
