#pragma once

// フレームワーク
#include "MAGI.h"

//-------------------------------------------
// 操作クラス
//-------------------------------------------
#include "Feature/GameInputSystem/GameInputSystem.h"

//-------------------------------------------
// シーンステート管理クラス
//-------------------------------------------
#include "Feature/SceneStateController/CombatSceneController/CombatSceneController.h"

//-------------------------------------------
// ステージデータ
//-------------------------------------------

#include "Feature/CombatStage/Data/CombatStageData.h"

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
#include "Feature/Effect/System/GameEffectManager/GameEffectManager.h"
#include "Feature/Damage/Object/Manager/DamageObjectManager.h"
#include "Feature/Damage/CollisionSystem/DamageCollisionSystem.h"
#include "Feature/GameUI/System/Manager/GameUIManager.h"

#include "MechAnimation/Container/MechAnimationContainer.h"

//-------------------------------------------
// エディタークラス
//-------------------------------------------
#include "MechAnimation/Edit/MechAnimationEdit.h"
#include "Feature/CombatStage/Editor/CombatStageEditor.h"

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
	void LoadResource();
private:
	// 入力システム
	std::unique_ptr<GameInputSystem> inputSys_;
	// シーン管理クラス
	std::unique_ptr<CombatSceneController> sceneController_;
	// ステージデータ
	std::unique_ptr<CombatStageData> stageData_;

	// シーンカメラの参照ポインタ
	Magi::TPSCamera3D* camera_;
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
	// 機体アニメーションコンテナクラス
	std::unique_ptr<MechAnimationContainer> mechAnimationContainer_;
	// UIマネージャ
	std::unique_ptr<GameUIManager> gameUIManger_;

	// 機体アニメーション作成クラス
	std::unique_ptr<MechAnimationEdit> mechAnimationEdit_;
	// ステージ作成クラス
	std::unique_ptr<CombatStageEditor> stageEditor_;

	// DirectionalLight
	DirectionalLight directionalLight_{};
};