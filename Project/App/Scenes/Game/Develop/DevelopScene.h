#pragma once

// フレームワーク
#include "MAGI.h"

//-------------------------------------------
// 操作クラス
//-------------------------------------------
#include "Feature/GameInputSystem/GameInputSystem.h"

//-------------------------------------------
// シーンオブジェクト
//-------------------------------------------

// カメラ
#include "Feature/TPSCamera3D/TPSCamera3D.h"

// パイロット
#include "Feature/Pilot/Pilot.h"

// ボス
#include "Feature/Boss/Boss.h"

//-------------------------------------------
// シーンに配置するマネージャ
//-------------------------------------------
#include "GameEffects/System/GameEffectManager/GameEffectManager.h"

#include "GameObject/Damage/Object/Manager/DamageObjectManager.h"
#include "GameObject/Damage/CollisionSystem/DamageCollisionSystem.h"

#include "MechAnimation/Edit/MechAnimationEdit.h"
#include "MechAnimation/Container/MechAnimationContainer.h"

/// <summary>
/// 開発用シーン
/// </summary>
class DevelopScene:public Magi::BaseScene {
public:
	using BaseScene::BaseScene;
	~DevelopScene()override = default;

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;

private:
	// 入力システム
	std::unique_ptr<GameInputSystem> inputSys_;

	// シーンカメラの参照ポインタ
	TPSCamera3D* camera_;

	// パイロット
	std::unique_ptr<Pilot> pilot_;
	// ボス
	std::unique_ptr<Boss> boss_;

	// エフェクトマネージャ
	std::unique_ptr<GameEffectManager> gameEffectManager_;
	// コリジョンシステム
	std::unique_ptr<DamageCollisionSystem> damageCollisionSystem_;
	// 攻撃オブジェクトマネージャ
	std::unique_ptr<DamageObjectManager> damageObjectManager_;

	// 機体アニメーション作成クラス
	std::unique_ptr<MechAnimationEdit> mechAnimationEdit_;
	// 機体アニメーションコンテナクラス
	std::unique_ptr<MechAnimationContainer> mechAnimationContainer_;

	// DirectionalLight
	DirectionalLight directionalLight_{};
};