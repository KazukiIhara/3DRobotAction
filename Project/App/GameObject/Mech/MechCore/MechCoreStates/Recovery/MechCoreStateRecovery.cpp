#include "MechCoreStateRecovery.h"

#include "GameObject/Mech/MechCore/MechCore.h"

void MechCoreStateRecovery::Enter(MechCore* mechCore) {
	// 移動速度を0にする
	mechCore->GetMovementComponent()->ResetVelocity();
}

void MechCoreStateRecovery::Update(MechCore* mechCore) {

}

void MechCoreStateRecovery::Exit(MechCore* mechCore) {

}
