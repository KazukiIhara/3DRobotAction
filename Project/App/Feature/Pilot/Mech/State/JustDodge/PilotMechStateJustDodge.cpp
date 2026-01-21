#include "PilotMechStateJustDodge.h"

#include "Feature/Pilot/Mech/PilotMech.h"
#include "Feature/GameInputSystem/GameInputSystem.h"
#include "Feature/Pilot/Mech/System/Move/PilotMechMoveSystem.h"

#include "MAGI.h"
#include "Math/Utility/MathUtility.h"
#include "MAGIUitility/MAGIUtility.h"

#include "Feature/Effect/System/GameEffectManager/GameEffectManager.h"

#include "Feature/Effect/JustDodgeEffect/JustDodgeEffect.h"

using namespace Magi;
using namespace MAGIMath;
using namespace MAGIUtility;

void PilotMechStateJustDodge::Enter(PilotMech* mech) {
	// タイマーセット
	timer_ = MAGISYSTEM::GetParameterValue<float>({ "PilotMechStateParam","JustDodge","Time" });

	// ジャスト回避エフェクト発生
	const Vector3 worldPos = mech->GetCenterPos();
	std::unique_ptr<JustDodgeEffect> dodgeEffect = std::make_unique<JustDodgeEffect>(worldPos);
	mech->GetGameEffectManager()->Add(std::move(dodgeEffect));

}

void PilotMechStateJustDodge::Update(PilotMech* mech) {



	// タイマー更新(生デルタタイムを使う)
	timer_ -= MAGISYSTEM::GetRawDeltaTime();
	timer_ = std::max(0.0f, timer_);

	// ステート更新
	if (timer_ <= 0.0f) {
		mech->ChangeState(PilotMech::State::Move);
	}
}

void PilotMechStateJustDodge::Exit(PilotMech* mech) {

}