#include "BossMechRightArm.h"

#include "MAGI.h"

#include "3D/Transform3D/Transform3D.h"
#include "GameObject/Boss/Mech/BossMech.h"

using namespace Magi;

BossMechRightArm::BossMechRightArm(const BossMechBaseArm::InitParam& param, BossMech* mech) :
	BossMechBaseArm(mech) {

	side_ = Side::Right;

	// 初期化パラメータを受け取る
	const std::string sideStr = "Right";
	upperModelName_ = param.upperModelName + sideStr;
	lowerModelName_ = param.lowerModelName + sideStr;
	handModelName_ = param.handModelName + sideStr;

	// モデル読み込みDrawer作成
	MAGISYSTEM::LoadCreateModel(upperModelName_);
	MAGISYSTEM::LoadCreateModel(lowerModelName_);
	MAGISYSTEM::LoadCreateModel(handModelName_);

}
