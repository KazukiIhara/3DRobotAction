#include "PilotMechStateJustDodge.h"

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

void PilotMechStateJustDodge::Enter([[maybe_unused]] PilotMech* mech) {
	// タイマーセット
	timer_ = MAGISYSTEM::GetParameterValue<float>({ "PilotMechStateParam","JustDodge","Time" });

	// ジャスト回避エフェクト発生
	const Vector3 worldPos = mech->GetCenterPos();
	std::unique_ptr<JustDodgeEffect> dodgeEffect = std::make_unique<JustDodgeEffect>(worldPos);
	mech->GetGameEffectManager()->Add(std::move(dodgeEffect));

	// 初速
	const float firstSpeed = MAGISYSTEM::GetParameterValue<float>({ "PilotMechStateParam","JustDodge","FirstSpeed" });

	// 加速量を0にする
	auto ms = mech->GetMoveSystem();
	ms->SetAcc(0.0f);
	// 初速と最大速度をセット
	ms->SetSpeed(firstSpeed);
	ms->SetMaxSpeed(firstSpeed);

}

void PilotMechStateJustDodge::Update([[maybe_unused]] PilotMech* mech) {
	// デルタタイムを取得
	const float dt = MAGISYSTEM::GetDeltaTime();

	// 入力取得
	auto commandPair = mech->GetInputSys()->GetPilotCommand();
	// 移動入力なしでIdleに遷移
	if (commandPair.first) {
		auto command = commandPair.second;
		// 左手攻撃コマンドでジャスト回避攻撃
		if (command.attackL) {
			mech->ChangeState(PilotMech::State::JustDodgeAttack);
			return;
		}
	}

	// デルタタイムをスローにする処理
	const float time = MAGISYSTEM::GetParameterValue<float>({ "PilotMechStateParam","JustDodge","Time" });
	const float deltaMulS = MAGISYSTEM::GetParameterValue<float>({ "PilotMechStateParam","JustDodge","DeltaMulStart" });
	const float deltaMulE = MAGISYSTEM::GetParameterValue<float>({ "PilotMechStateParam","JustDodge","DeltaMulEnd" });
	SimpleAnimation<float> deltaTimeAnimation = SimpleAnimation<float>(deltaMulS, deltaMulE, EasingType::EaseInQuart);

	// ステート時間を0.0f~1.0fに変換
	const float t = 1.0f - timer_ / time;
	const float deltaMul = deltaTimeAnimation.GetValue(t);
	// デルタタイムに倍率を書ける
	MAGISYSTEM::SetDeltaTimeMultiplier(deltaMul);

	// 速度補間
	{
		auto ms = mech->GetMoveSystem();
		const float speedT = CalExpT(dt, time, 0.5f);
		// 通常移動の最大速度まで補間
		const float maxMoveSpeed = MAGISYSTEM::GetParameterValue<float>({ "PilotMechStateParam","Move","MaxSpeed" });
		const float currentSpeed = ms->GetSpeed();
		const float speed = Lerp(currentSpeed, maxMoveSpeed, speedT);
		ms->SetSpeed(speed);
	}

	// タイマー更新(生デルタタイムを使う)
	timer_ -= MAGISYSTEM::GetRawDeltaTime();
	timer_ = std::max(0.0f, timer_);

	// ステート更新
	if (timer_ <= 0.0f) {
		mech->ChangeState(PilotMech::State::Move);
	}
}

void PilotMechStateJustDodge::Exit([[maybe_unused]] PilotMech* mech) {
	// 1.0fに戻す
	MAGISYSTEM::SetDeltaTimeMultiplier(1.0f);
}