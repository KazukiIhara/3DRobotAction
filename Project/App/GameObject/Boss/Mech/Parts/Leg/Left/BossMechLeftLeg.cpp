#include "BossMechLeftLeg.h"

BossMechLeftLeg::BossMechLeftLeg(const BossMechBaseLeg::InitParam& param, BossMech* mech) :
	BossMechBaseLeg(param, mech) {

	side_ = Side::Left;

}