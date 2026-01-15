#include "BossMechRightLeg.h"

BossMechRightLeg::BossMechRightLeg(const BossMechBaseLeg::InitParam& param, BossMech* mech) :
	BossMechBaseLeg(param, mech) {

	side_ = Side::Right;

}
