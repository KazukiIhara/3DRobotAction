#include "BossMechBody.h"

#include "MAGI.h"

#include "3D/Transform3D/Transform3D.h"
#include "GameObject/Boss/Mech/BossMech.h"

using namespace Magi;

BossMechBody::BossMechBody(const BossMechBody::InitParam& param, BossMech* mech) {

}

void BossMechBody::Update() {

}

void BossMechBody::Draw() {

}

void BossMechBody::DebugDraw() {

}

Transform3D* BossMechBody::GetBodyTransform() {
	return bodyTrans_;
}
