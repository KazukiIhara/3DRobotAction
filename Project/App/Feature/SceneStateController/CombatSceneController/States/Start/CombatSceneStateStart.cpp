#define NOMINMAX

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

#include "Feature/SceneStateController/CombatSceneController/CombatSceneController.h"

#include "MAGI.h"

using namespace Magi;

void CombatSceneStateStart::Enter([[maybe_unused]] CombatSceneControl::StateContextRef ref) {
	timer_ = 1.0f;
}

void CombatSceneStateStart::Update([[maybe_unused]] CombatSceneControl::StateContextRef ref) {




	timer_ -= MAGISYSTEM::GetDeltaTime();
	timer_ = std::max(0.0f, timer_);
	if (timer_ <= 0.0f) {
		ref.cbc->ChangeState(CombatSceneController::State::Battle);
	}
}

void CombatSceneStateStart::Draw([[maybe_unused]] CombatSceneControl::StateContextRef ref) {

}

void CombatSceneStateStart::Exit([[maybe_unused]] CombatSceneControl::StateContextRef ref) {

}
