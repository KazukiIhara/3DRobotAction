#include "CombatSceneStateDropped.h"

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
#include "Feature/GameUI/System/Manager/GameUIManager.h"


// シーンコントローラ
#include "Feature/SceneStateController/CombatSceneController/CombatSceneController.h"


// フレームワーク
#include "MAGI.h"

using namespace Magi;


void CombatSceneStateDropped::Enter(CombatSceneControl::StateContextRef ref) {
	// パラメータ作成
	MAGISYSTEM::AddParameterData({ "CombatSceneParam","Dropped","Time" }, ParamType::Float);

	// テクスチャロード
	MAGISYSTEM::LoadTexture("UnitDestroyed.png");

	// タイマーセット
	timer_ = MAGISYSTEM::GetParameterValue<float>({ "CombatSceneParam","Dropped","Time" });

	// スプライト用マテリアルをセット
	spriteMat_.textureName = "UnitDestroyed.png";
}

void CombatSceneStateDropped::Update(CombatSceneControl::StateContextRef ref) {
	// デルタタイム取得
	const float dt = MAGISYSTEM::GetDeltaTime();
	// ステートタイマー更新
	timer_ -= dt;
	if (timer_ <= 0.0f) {
		// 戦闘ステート終了
		ref.cbc->End();
	}

}

void CombatSceneStateDropped::Draw(CombatSceneControl::StateContextRef ref) {
	MAGISYSTEM::DrawSprite(sprite_, spriteMat_);
}

void CombatSceneStateDropped::Exit(CombatSceneControl::StateContextRef ref) {

}
