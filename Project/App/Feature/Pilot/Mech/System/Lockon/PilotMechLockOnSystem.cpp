#include "PilotMechLockOnSystem.h"

PilotMechLockOnSystem::PilotMechLockOnSystem(PilotMech* mech) {
	mech_ = mech;

}

void PilotMechLockOnSystem::Update() {

}

ILockOnTarget* PilotMechLockOnSystem::GetTarget() {
	return target_;
}

void PilotMechLockOnSystem::SetBoss(ILockOnTarget* bossMech) {
	target_ = bossMech;
}
