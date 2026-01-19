#include "PilotMechDirectionSystem.h"

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
	const Vector2 currentDir = Vector2(dir.x, dir.z);
	const float yaw = std::atan2(currentDir.x, currentDir.y);
	const Quaternion targetQ = MakeRotateAxisAngleQuaternion({ 0.0f,1.0f,0.0f }, yaw);
	mech_->GetModelTransform()->SetQuaternion(targetQ);
}