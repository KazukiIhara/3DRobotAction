#include "BossMechBaseArm.h"

#include "MAGI.h"

#include "3D/Transform3D/Transform3D.h"
#include "GameObject/Boss/Mech/BossMech.h"

using namespace Magi;

BossMechBaseArm::BossMechBaseArm(const BossMechBaseArm::InitParam& param, BossMech* mech) {

}

void BossMechBaseArm::Update() {

}

void BossMechBaseArm::Draw() {

}

Transform3D* BossMechBaseArm::GetUpperTransform() {
	return nullptr;
}

Transform3D* BossMechBaseArm::GetLowerTransform() {
	return nullptr;
}

Transform3D* BossMechBaseArm::GetHandTransform() {
	return nullptr;
}