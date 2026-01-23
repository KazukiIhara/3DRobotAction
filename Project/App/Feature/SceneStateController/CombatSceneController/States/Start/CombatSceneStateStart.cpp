#include "CombatSceneStateStart.h"

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

void CombatSceneStateStart::Enter([[maybe_unused]] CombatSceneControl::ContextRef ref) {
	// 一旦ここでカメラをプレイヤーに紐づける
	auto pilotTransform = ref.pilot->GetMech()->GetPartsTransform(MechAnimation::TransType::Body);
	ref.camera->SetFollowTransform(pilotTransform);

}

void CombatSceneStateStart::Update([[maybe_unused]] CombatSceneControl::ContextRef ref) {

}

void CombatSceneStateStart::Draw([[maybe_unused]] CombatSceneControl::ContextRef ref) {

}

void CombatSceneStateStart::Exit([[maybe_unused]] CombatSceneControl::ContextRef ref) {

}
