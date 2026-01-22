#include "PilotMechModelDirSystem.h"

#include "Math/Utility/MathUtility.h"
#include "Feature/Pilot/Mech/PilotMech.h"
#include "MAGI.h"
using namespace Magi;

PilotMechModelDirSystem::PilotMechModelDirSystem(PilotMech* mech) {
	mech_ = mech;

}

void PilotMechModelDirSystem::Update() {
	// 進行方向に向ける
	TurnToDirection();

}

void PilotMechModelDirSystem::TurnToDirection() {
	const Vector3 dir = mech_->GetMoveSystem()->GetDir();
	const Quaternion targetQ = DirectionToQuaternionYaw_s(dir);
	mech_->GetModelTransform()->SetQuaternion(targetQ);
}