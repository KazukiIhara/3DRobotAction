#define NOMINMAX

#include "CombatSceneStateBattle.h"

//-------------------------------------------
// 操作クラス
//-------------------------------------------
#include "Feature/GameInputSystem/GameInputSystem.h"

// カメラ
#include "Feature/TPSCamera3D/TPSCamera3D.h"

// パイロット
#include "Feature/Pilot/Pilot.h"

// ボス
#include "Feature/Boss/Boss.h"

//-------------------------------------------
// マネージャ
//-------------------------------------------
#include "Feature/Effect/System/GameEffectManager/GameEffectManager.h"
#include "Feature/Damage/Object/Manager/DamageObjectManager.h"
#include "Feature/Damage/CollisionSystem/DamageCollisionSystem.h"

#include "Feature/SceneStateController/CombatSceneController/CombatSceneController.h"

#include "MAGI.h"

using namespace Magi;

void CombatSceneStateBattle::Enter([[maybe_unused]] CombatSceneControl::StateContextRef ref) {
	// カメラをプレイヤーに紐づける
	auto pilotTransform = ref.pilot->GetMech()->GetPartsTransform(MechAnimation::TransType::Body);
	ref.camera->SetFollowTransform(pilotTransform);

}

void CombatSceneStateBattle::Update([[maybe_unused]] CombatSceneControl::StateContextRef ref) {

	// ボスのHPが0になったら終了
	const int32_t bossHP = ref.boss->GetMech()->GetStatus()->GetHP();
	if (bossHP <= 0) {
		// 終了
		ref.cbc->ChangeState(CombatSceneController::State::End);
	}



}

void CombatSceneStateBattle::Draw([[maybe_unused]] CombatSceneControl::StateContextRef ref) {

}

void CombatSceneStateBattle::Exit([[maybe_unused]] CombatSceneControl::StateContextRef ref) {

}
