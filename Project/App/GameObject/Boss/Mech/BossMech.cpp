#include "BossMech.h"

#include "GameObject/Boss/Mech/State/BossMechBaseState.h"
#include "GameObject/Boss/Mech/State/Idle/BossMechStateIdle.h"

BossMech::BossMech(const BossMech::InitParam& initParam) {

}

void BossMech::Update() {
	// 全パーツを更新
	head_->Update();
	body_->Update();
	rightArm_->Update();
	leftArm_->Update();
	leftLeg_->Update();
	rightLeg_->Update();
}

void BossMech::Draw() {
	head_->Draw();
	body_->Draw();
	rightArm_->Draw();
	leftArm_->Draw();
	leftLeg_->Draw();
	rightLeg_->Draw();
}

void BossMech::ChangeState(BossMech::BossMechState nextState) {

}

BossMechBaseState* BossMech::GetState(BossMechState state) {
	return nullptr;
}
