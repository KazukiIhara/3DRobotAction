#pragma once

#include <memory>
#include <array>

#include "MAGI.h"

#include "GameObject/SceneUIManager/SceneUIManager.h"

//-------------------------------------------
// シーンオブジェクト
//-------------------------------------------
#include "GameObject/AIPlayer/AIPlayer.h"
#include "GameObject/Enemy/Enemy.h"
#include "GameObject/PlayerUI/PlayerUI.h"

#include "GameObject/AttackCollisionManager/AttackCollisionManager.h"
#include "GameObject/AttackObjectManager/AttackObjectManager.h"

/// <summary>
/// タイトルシーン
/// </summary>
class TitleScene :public Magi::BaseScene {
public:
	using BaseScene::BaseScene; // 親クラスのコンストラクタをそのまま継承
	~TitleScene()override = default;

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;

private:
	SpriteData bgData_;
	SpriteMaterialData bgMatData_;
	SpriteData startData_;
	SpriteMaterialData startMatData_;
	SpriteData exitData_;
	SpriteMaterialData exitMatData_;


	// デモプレイ用

	// プレイヤー
	std::unique_ptr<AIPlayer> aiPlayer_;

	// 敵
	std::unique_ptr<Enemy> enemy_;

	// プレイヤーの情報を描画するUI
	std::unique_ptr<PlayerUI> playerUI_;

	// 攻撃判定マネージャ
	std::unique_ptr<AttackCollisionManager> attackCollisionManager_;

	// 弾のマネージャ
	std::unique_ptr<AttackObjectManager> attackObjectManger_;


	// DirectionalLight
	DirectionalLight directionalLight_{};

	// ポストエフェクトの用の変数
	float vignetteScale_ = 18.0f;
	float vignetteFalloff_ = 0.8f;
	Vector2 radialBlurCenter_ = { 0.5f,0.5f };
	float radialBlurWidth_ = 0.01f;
	float gaussianSigma_ = 0.5f;


};

