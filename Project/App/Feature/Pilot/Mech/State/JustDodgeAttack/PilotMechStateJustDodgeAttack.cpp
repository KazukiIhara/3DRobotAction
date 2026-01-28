#include "PilotMechStateJustDodgeAttack.h"

#include "Feature/Pilot/Mech/PilotMech.h"
#include "Feature/GameInputSystem/GameInputSystem.h"

#include "MAGI.h"
#include "Math/Utility/MathUtility.h"
#include "MAGIUitility/MAGIUtility.h"

#include "Feature/Effect/System/GameEffectManager/GameEffectManager.h"

#include "Feature/Effect/JustDodgeEffect/JustDodgeEffect.h"

using namespace Magi;
using namespace MAGIMath;
using namespace MAGIUtility;

void PilotMechStateJustDodgeAttack::Enter([[maybe_unused]] PilotMech* mech) {
	// フェーズをチャージショットにする
	phase_ = Phase::ChargeShot;

	// タイマーセット
	timer_ = MAGISYSTEM::GetParameterValue<float>({ "PilotMechStateParam","JustDodgeAttack","Charge","Time" });

	// 射撃までのアニメーションを再生


	// 移動速度処理
	auto ms = mech->GetMoveSystem();
	// 加速量をセット
	const float jdaAcc = MAGISYSTEM::GetParameterValue<float>({ "PilotMechStateParam","JustDodge","FirstSpeed" });
	// ジャスト回避時の入力速度を加速量にして、大体1秒で速度が0になるようにする
	ms->SetAcc(-jdaAcc);

}

void PilotMechStateJustDodgeAttack::Update([[maybe_unused]] PilotMech* mech) {
	// タイマー更新
	const float dt = MAGISYSTEM::GetDeltaTime();
	timer_ -= dt;
	timer_ = std::max(0.0f, timer_);

	// フェーズに合わせて更新
	switch (phase_) {
		case PilotMechStateJustDodgeAttack::Phase::ChargeShot:
			UpdateChargeShot(mech);
			break;
		case PilotMechStateJustDodgeAttack::Phase::EndLag:
			UpdateEndLag(mech);
			break;
	}

}

void PilotMechStateJustDodgeAttack::Exit([[maybe_unused]] PilotMech* mech) {

}

void PilotMechStateJustDodgeAttack::UpdateChargeShot([[maybe_unused]] PilotMech* mech) {
	// チャージ中のエフェクト処理

	// タイマー終了
	if (timer_ <= 0.0f) {
		// 攻撃
		mech->GetWeapon("BeamCannon")->Attack();
		// 硬直用タイマーセット
		timer_ = MAGISYSTEM::GetParameterValue<float>({ "PilotMechStateParam","JustDodgeAttack","EndLag","Time" });
		// 硬直アニメーション再生

		// 硬直フェーズへ
		phase_ = Phase::EndLag;
	}
}

void PilotMechStateJustDodgeAttack::UpdateEndLag([[maybe_unused]] PilotMech* mech) {
	// 射撃後のエフェクト


	// タイマー終了でステート終了
	if (timer_ <= 0.0f) {
		mech->ChangeState(PilotMech::State::Move);
	}
}
