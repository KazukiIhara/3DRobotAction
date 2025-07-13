#include "MechAttackComponent.h"

#include "GameObject/Mech/MechCore/MechCore.h"

#include "MAGI.h"
#include "MAGIAssert/MAGIAssert.h"

using namespace MAGIMath;

MechAttackComponent::MechAttackComponent() {
	

}


void MechAttackComponent::AttackLeftHand(MechCore* mechCore) {
	// 左手の武器を見に行く
	const WeaponType type = mechCore->GetLeftHandWeapon()->GetType();

}

void MechAttackComponent::AttackRightHand(MechCore* mechCore) {

}
