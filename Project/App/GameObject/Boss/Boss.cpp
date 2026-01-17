#include "Boss.h"

Boss::Boss(
	DamageObjectManager* damageObjectManager,
	GameEffectManager* gameEffectManager,
	MechCore* playerMech
) {

	// パラメータ作成
	BossMech::InitParam initParam{};
	initParam.head.modelName = "BossMechHead";
	initParam.body.modelName = "BossMechBody";
	initParam.arm.upperModelName = "BossMechUpperArm";
	initParam.arm.lowerModelName = "BossMechLowerArm";
	initParam.arm.handModelName = "BossMechHand";

	// 機体の作成
	mech_ = std::make_unique<BossMech>(
		initParam,
		damageObjectManager,
		gameEffectManager,
		playerMech
	);
	// AIの作成


}


void Boss::Update() {
	// 機体の更新
	mech_->Update(flag_.isDebugDraw);

}

void Boss::Draw() {
	// 機体の描画
	mech_->Draw();
}

BossMech* Boss::GetMech() {
	return mech_.get();
}

bool Boss::GetIsAIActive()const {
	return flag_.isAIActive;
}

void Boss::SetIsAIActive(bool isActive) {
	flag_.isAIActive = isActive;
}

void Boss::SwitchDebugDraw() {
	flag_.isDebugDraw = !flag_.isDebugDraw;
}

void Boss::SwitchAIActive() {
	flag_.isAIActive = !flag_.isAIActive;
}

void Boss::SwitchIsPause() {
	flag_.isPause = !flag_.isPause;
}