#include "BossMechLeftArm.h"

#include "MAGI.h"

#include "3D/Transform3D/Transform3D.h"
#include "GameObject/Boss/Mech/BossMech.h"

using namespace Magi;

BossMechLeftArm::BossMechLeftArm(const BossMechBaseArm::InitParam& param, BossMech* mech) :
	BossMechBaseArm(param, mech) {
	side_ = Side::Left;

}