#pragma once

#include <memory>
#include <array>

#include "MAGI.h"
#include "MAGIUitility/MAGIUtility.h"

using namespace MAGIUtility;

//-------------------------------------------
// シーンオブジェクト
//-------------------------------------------
#include "GameObject/Player/Player.h"
#include "GameObject/Enemy/Enemy.h"

#include "GameObject/AttackCollisionManager/AttackCollisionManager.h"
#include "GameObject/BulletManager/BulletManager.h"

/// <summary>
/// 終了判定
/// </summary>
enum class FinishJudgment {
	Player,
	Enemy,
	Draw,
};

/// <summary>
/// プレイシーンの情報構造体
/// </summary>
struct PlaySceneInfo {
	int32_t battleTime = 90;
	FinishJudgment judge;
	bool finish = false;
	bool isPause = false;

};

/// <summary>
/// プレイシーンの状態
/// </summary>
enum class PlaySceneState {
	Init,
	Play,
	Finish,
};

/// <summary>
/// ゲームプレイシーン
/// </summary>
/// <typeparam name="Data"></typeparam>
class PlayScene:public BaseScene {
public:
	using BaseScene::BaseScene; // 親クラスのコンストラクタをそのまま継承
	~PlayScene()override = default;

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;

private:
	// カメラ
	std::weak_ptr<Camera3D> mainCamera_;

	// DirectionalLight
	DirectionalLight directionalLight_{};

	//----------------------------------------- 
	// シーンオブジェクト
	//-----------------------------------------

	// プレイヤー
	std::unique_ptr<Player> player_;

	// 敵
	std::unique_ptr<Enemy> enemy_;

	// 終了時UI
	SpriteData finishSpriteData_;
	SpriteMaterialData finishSpriteMatData_;

	//----------------------------------------- 
	// マネージャ
	//-----------------------------------------

	// 攻撃判定マネージャ
	std::unique_ptr<AttackCollisionManager> attackCollisionManager_;

	// 弾のマネージャ
	std::unique_ptr<BulletManager> bulletManger_;


	// ポストエフェクトの用の変数
	float vignetteScale_ = 18.0f;
	float vignetteFalloff_ = 0.8f;
	Vector2 radialBlurCenter_ = { 0.5f,0.5f };
	float radialBlurWidth_ = 0.01f;
	float gaussianSigma_ = 0.5f;

	// プレイシーンの情報
	PlaySceneInfo info;

	// 戦闘タイマー
	float tempBattleTime_ = 0.0;
	int32_t kMaxBattleTime_ = 90;

	// シーン終了演出のタイマー
	float finishSceneTimer_ = 3.0f;

	// 
	// デバッグ用
	// 

	// 板ポリエフェクトのパラメータ
	PlaneEffectParam planeEffect_;

};