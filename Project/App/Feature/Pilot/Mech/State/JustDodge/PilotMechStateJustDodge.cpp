#include "PilotMechStateJustDodge.h"

#include "Feature/Pilot/Mech/PilotMech.h"
#include "Feature/GameInputSystem/GameInputSystem.h"

#include "MAGI.h"
#include "Math/Utility/MathUtility.h"
#include "MAGIUitility/MAGIUtility.h"

#include "Feature/Effect/System/GameEffectManager/GameEffectManager.h"

#include "Feature/Effect/JustDodgeEffect/JustDodgeEffect.h"

#include "Feature/Pilot/Mech/Weapon/MachineGun/PilotMechWeaponMachineGun.h"

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

	// ジャスト回避アニメーション再生
	mech->GetAnimator()->PlayAnimation("Pilot_JustDodge", 0.2f, 0.0f, EasingType::EaseOutCubic);

	// 初速
	const float firstSpeed = MAGISYSTEM::GetParameterValue<float>({ "PilotMechStateParam","JustDodge","FirstSpeed" });

	// 加速量を0にする
	auto ms = mech->GetMoveSystem();
	ms->SetAcc(0.0f);

	// 初速と最大速度をセット
	ms->SetSpeed(firstSpeed);
	ms->SetMaxSpeed(firstSpeed);

	// マシンガンの残弾を回復
	const int32_t reloadAmmoNum = MAGISYSTEM::GetParameterValue<int32_t>({ "PilotMechStateParam","JustDodge","ReloadAmmoNum" });
	auto weapon = dynamic_cast<PilotMechWeaponMachineGun*>(mech->GetWeapon("MachineGun"));
	weapon->AddAmmo(reloadAmmoNum);

}

void PilotMechStateJustDodge::Update([[maybe_unused]] PilotMech* mech) {
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