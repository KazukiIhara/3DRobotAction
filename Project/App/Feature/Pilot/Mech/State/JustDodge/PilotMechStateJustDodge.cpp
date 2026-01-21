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
	const Vector3 worldPos = mech->GetCenterPos();
	std::unique_ptr<JustDodgeEffect> dodgeEffect = std::make_unique<JustDodgeEffect>(worldPos);
	mech->GetGameEffectManager()->Add(std::move(dodgeEffect));

}

void PilotMechStateJustDodge::Update(PilotMech* mech) {

}

void PilotMechStateJustDodge::Exit(PilotMech* mech) {

}