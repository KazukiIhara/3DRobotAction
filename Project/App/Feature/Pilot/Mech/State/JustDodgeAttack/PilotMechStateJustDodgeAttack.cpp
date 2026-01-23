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
	mech->GetWeapon("BeamCannonRifle");
}

void PilotMechStateJustDodgeAttack::Update([[maybe_unused]] PilotMech* mech) {

}

void PilotMechStateJustDodgeAttack::Exit([[maybe_unused]] PilotMech* mech) {

}
