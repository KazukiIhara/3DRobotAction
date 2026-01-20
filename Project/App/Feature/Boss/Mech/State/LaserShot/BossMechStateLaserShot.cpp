#include "BossMechStateLaserShot.h"

#include "MAGI.h"

#include "Feature/Boss/Mech/BossMech.h"

using namespace Magi;

void BossMechStateLaserShot::Enter([[maybe_unused]] BossMech* mech) {
	const float timeStartUp = MAGISYSTEM::GetParameterValue<float>({ "BossMechState","LaserShot","TimeStartUp" });
	timer_ = timeStartUp;
}

void BossMechStateLaserShot::Update([[maybe_unused]] BossMech* mech) {
	// デルタタイムを取得
	const float dt = MAGISYSTEM::GetDeltaTime();
	switch (phase_) {
	case BossMechStateLaserShot::Phase::StartUp:
		UpdateStartUp(dt, mech);
		break;
	case BossMechStateLaserShot::Phase::EndLag:
		UpdateEndLag(dt, mech);
		break;
	}
}

void BossMechStateLaserShot::Exit([[maybe_unused]] BossMech* mech) {

}

void BossMechStateLaserShot::UpdateStartUp(float dt, BossMech* mech) {
	// 攻撃準備アニメーション
	const float timeStartUp = MAGISYSTEM::GetParameterValue<float>({ "BossMechState","LaserShot","TimeStartUp" });


	// タイマー更新
	timer_ -= dt;
	timer_ = std::max(0.0f, timer_);
	if (timer_ == 0.0f) {
		// 攻撃
		mech->GetWeapon("LaserGun");
		const float timeEndLag = MAGISYSTEM::GetParameterValue<float>({ "BossMechState","LaserShot","TimeEndLag" });
		timer_ = timeEndLag;
		phase_ = Phase::EndLag;
	}
}

void BossMechStateLaserShot::UpdateEndLag(float dt, BossMech* mech) {
	// 攻撃終了アニメーション
	const float timeEndLag = MAGISYSTEM::GetParameterValue<float>({ "BossMechState","LaserShot","TimeEndLag" });


	// タイマー更新
	timer_ -= dt;
	timer_ = std::max(0.0f, timer_);
	// タイマー0で待機状態に遷移
	if (timer_ == 0.0f) {
		mech->ChangeState(BossMech::State::Idle);
	}
}
