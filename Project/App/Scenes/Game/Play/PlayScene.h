#pragma once

#include <memory>
#include <array>

#include "MAGI.h"
#include "MAGIUitility/MAGIUtility.h"
#include "SimpleAnimation/SimpleAnimation.h"

using namespace MAGIUtility;

//-------------------------------------------
// シーンオブジェクト
//-------------------------------------------
#include "GameObject/Player/Player.h"
#include "GameObject/Enemy/Enemy.h"

#include "GameObject/AttackCollisionManager/AttackCollisionManager.h"
#include "GameObject/AttackObjectManager/AttackObjectManager.h"

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
	bool isPause = false;
};

/// <summary>
/// プレイシーンの状態
/// </summary>
enum class PlaySceneState {
	Start,
	Play,
	Finish,
};

// 開始アニメーションのフェーズ
enum class StartAnimPhase {
	In,
	Stay,
	Out,
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
	std::unique_ptr<AttackObjectManager> attackObjectManger_;

	// シーンのステート
	PlaySceneState playSceneState_;

	// ポストエフェクトの用の変数
	float vignetteScale_ = 18.0f;
	float vignetteFalloff_ = 0.8f;
	magi::Vector2 radialBlurCenter_ = { 0.5f,0.5f };
	float radialBlurWidth_ = 0.01f;
	float gaussianSigma_ = 0.5f;

	// プレイシーンの情報
	PlaySceneInfo info;


	// プレイシーン開始演出のタイマー
	float startSceneTimer_ = 0.0f;
	const float kStartSceneTime_ = 3.0f;
	const float kStartSceneAnimTime_ = 1.2f;
	const float kStartSceneAnimFTime_ = 0.8f;
	// 開始演出の状態
	StartAnimPhase startAnimPhase_ = StartAnimPhase::In;

	// 戦闘タイマー
	float tempBattleTime_ = 0.0;
	int32_t kMaxBattleTime_ = 90;

	// シーン終了演出のタイマー
	float finishSceneTimer_ = 3.0f;


	// 開始シーンのスプライトデータ

	// BATTLE START
	SpriteData battleUiB_;
	SpriteData battleUiA_;
	SpriteData battleUiT_[2];
	SpriteData battleUiL_;
	SpriteData battleUiE_;

	SpriteData startUiS_;
	SpriteData startUiT_[2];
	SpriteData startUiA_;
	SpriteData startUiR_;

	// 開始シーンのスプライトマテリアル
	SpriteMaterialData battleUiMatB_;
	SpriteMaterialData battleUiMatA_;
	SpriteMaterialData battleUiMatT_[2];
	SpriteMaterialData battleUiMatL_;
	SpriteMaterialData battleUiMatE_;

	SpriteMaterialData startUiMatS_;
	SpriteMaterialData startUiMatT_[2];
	SpriteMaterialData startUiMatA_;
	SpriteMaterialData startUiMatR_;


	// UIアニメーションスタート座標
	magi::Vector2 battleUiBPosS_{};
	magi::Vector2 battleUiAPosS_{};
	magi::Vector2 battleUiTPosS_[2]{};
	magi::Vector2 battleUiLPosS_{};
	magi::Vector2 battleUiEPosS_{};

	magi::Vector2 startUiSPosS_{};
	magi::Vector2 startUiTPosS_[2]{};
	magi::Vector2 startUiAPosS_{};
	magi::Vector2 startUiRPosS_{};

	// UIアニメーション終了座標
	magi::Vector2 battleUiBPosE_{};
	magi::Vector2 battleUiAPosE_{};
	magi::Vector2 battleUiTPosE_[2]{};
	magi::Vector2 battleUiLPosE_{};
	magi::Vector2 battleUiEPosE_{};

	magi::Vector2 startUiSPosE_{};
	magi::Vector2 startUiTPosE_[2]{};
	magi::Vector2 startUiAPosE_{};
	magi::Vector2 startUiRPosE_{};


	// 現在のUIの座標
	magi::Vector2 battleUiBPos_{};
	magi::Vector2 battleUiAPos_{};
	magi::Vector2 battleUiTPos_[2]{};
	magi::Vector2 battleUiLPos_{};
	magi::Vector2 battleUiEPos_{};

	magi::Vector2 startUiSPos_{};
	magi::Vector2 startUiTPos_[2]{};
	magi::Vector2 startUiAPos_{};
	magi::Vector2 startUiRPos_{};


	// UIアニメーション
	SimpleAnimation<magi::Vector2> animBattleB_;
	SimpleAnimation<magi::Vector2> animBattleA_;
	SimpleAnimation<magi::Vector2> animBattleT_[2];
	SimpleAnimation<magi::Vector2> animBattleL_;
	SimpleAnimation<magi::Vector2> animBattleE_;

	SimpleAnimation<magi::Vector2> animStartS_;
	SimpleAnimation<magi::Vector2> animStartT_[2];
	SimpleAnimation<magi::Vector2> animStartA_;
	SimpleAnimation<magi::Vector2> animStartR_;


	// UIアニメーション最終座標
	magi::Vector2 battleUiBPosF_{};
	magi::Vector2 battleUiAPosF_{};
	magi::Vector2 battleUiTPosF_[2]{};
	magi::Vector2 battleUiLPosF_{};
	magi::Vector2 battleUiEPosF_{};

	magi::Vector2 startUiSPosF_{};
	magi::Vector2 startUiTPosF_[2]{};
	magi::Vector2 startUiAPosF_{};
	magi::Vector2 startUiRPosF_{};

	// UIアニメーション
	SimpleAnimation<magi::Vector2> animFBattleB_;
	SimpleAnimation<magi::Vector2> animFBattleA_;
	SimpleAnimation<magi::Vector2> animFBattleT_[2];
	SimpleAnimation<magi::Vector2> animFBattleL_;
	SimpleAnimation<magi::Vector2> animFBattleE_;

	SimpleAnimation<magi::Vector2> animFStartS_;
	SimpleAnimation<magi::Vector2> animFStartT_[2];
	SimpleAnimation<magi::Vector2> animFStartA_;
	SimpleAnimation<magi::Vector2> animFStartR_;

};