#include "Boss.h"

Boss::Boss(
	AttackObjectManager* attackObjectManager,
	GameEffectManager* gameEffectManager,
	MechCore* playerMech
) {
	// 機体の作成
	mech_ = std::make_unique<BossMech>(BossMech::InitParam{});
	// AIの作成


}


void Boss::Update() {
	// 機体の更新
	mech_->Update();

}

void Boss::Draw() {
	// 機体の描画
	mech_->Draw(flag_.isDebugDraw);

}

BossMech* Boss::GetMech() {
	return mech_.get();
}

void Boss::SwitchDebugDraw() {
	flag_.isDebugDraw = !flag_.isDebugDraw;
}

void Boss::SwitchAIActive() {
	flag_.isAIActive = !flag_.isAIActive;
}

bool Boss::GetIsAIActive()const {
	return flag_.isAIActive;
}

void Boss::SetIsAIActive(bool isActive) {
	flag_.isAIActive = isActive;
}