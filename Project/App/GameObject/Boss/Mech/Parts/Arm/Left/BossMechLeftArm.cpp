#include "BossMechLeftArm.h"

BossMechLeftArm::BossMechLeftArm(const BossMechBaseArm::InitParam& param, BossMech* mech) :
	BossMechBaseArm(param, mech) {

	side_ = Side::Left;

}