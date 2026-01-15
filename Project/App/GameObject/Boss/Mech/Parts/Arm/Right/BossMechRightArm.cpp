#include "BossMechRightArm.h"

BossMechRightArm::BossMechRightArm(const BossMechBaseArm::InitParam& param, BossMech* mech) :
	BossMechBaseArm(param, mech) {

	side_ = Side::Right;
}
