#include "Boss.h"

Boss::Boss(
	AttackObjectManager* attackObjectManager,
	GameEffectManager* gameEffectManager,
	MechCore* playerMech
) {

	// 機体の作成


	// AIの作成


}


void Boss::Update() {

}

void Boss::Draw() {

}

BossMech* Boss::GetMech() {
	return mech_.get();
}

bool Boss::GetIsAIActive()const {
	return false;
}