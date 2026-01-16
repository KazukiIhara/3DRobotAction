#include "BossMechStateLaserShot.h"

#include "MAGI.h"

#include "GameObject/Boss/Mech/BossMech.h"

using namespace Magi;

void BossMechStateLaserShot::Enter([[maybe_unused]] BossMech* mech) {
	time_ = MAGISYSTEM::GetParameterValue<float>({ "BossMechState","LaserShot","Time" });
}

void BossMechStateLaserShot::Update([[maybe_unused]] BossMech* mech) {
	// デルタタイムを取得
	const float dt = MAGISYSTEM::GetDeltaTime();
	// タイマー更新
	time_ -= dt;
	time_ = std::max(0.0f, time_);

	// タイマー0で待機状態に遷移
	if (time_ == 0.0f) {
		mech->ChangeState(BossMech::BossMechState::Idle);
	}

}

void BossMechStateLaserShot::Exit([[maybe_unused]] BossMech* mech) {

}