#include "BossMechBaseWeapon.h"

#include "MAGI.h"

#include "GameObject/AttackObjectManager/AttackObjectManager.h"
#include "GameEffects/System/GameEffectManager/GameEffectManager.h"

using namespace Magi;

BossMechBaseWeapon::BossMechBaseWeapon() {

}

Transform3D* BossMechBaseWeapon::GetTransfom() {
	return transform_;
}
