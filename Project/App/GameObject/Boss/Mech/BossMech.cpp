#include "BossMech.h"

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