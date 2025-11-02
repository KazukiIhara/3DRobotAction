#include "MechCoreStateRecovery.h"

#include "GameObject/Mech/MechCore/MechCore.h"

void MechCoreStateRecovery::Enter(MechCore* mechCore) {
	// 移動速度を0にする
	mechCore->GetMovementComponent()->SetMoveSpeed(0.0f);
}

void MechCoreStateRecovery::Update(MechCore* mechCore) {

}

void MechCoreStateRecovery::Exit(MechCore* mechCore) {

}
