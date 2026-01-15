#pragma once

// フレームワーク
#include "MAGI.h"

//-------------------------------------------
// シーンオブジェクト
//-------------------------------------------
#include "GameObject/Player/Player.h"
#include "GameObject/Boss/Boss.h"

//-------------------------------------------
// シーンに配置するマネージャ
//-------------------------------------------
#include "GameObject/AttackCollisionManager/AttackCollisionManager.h"
#include "GameObject/AttackObjectManager/AttackObjectManager.h"
#include "GameEffects/System/GameEffectManager/GameEffectManager.h"

/// <summary>
/// 開発用シーン
/// </summary>
class DevelopScene :public Magi::BaseScene {
public:
	using BaseScene::BaseScene;
	~DevelopScene()override = default;

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;

private:
	// プレイヤー
	std::unique_ptr<Player> player_;
	// ボス
	std::unique_ptr<Boss> boss_;

	// 攻撃判定マネージャ
	std::unique_ptr<AttackCollisionManager> attackCollisionManager_;
	// 弾のマネージャ
	std::unique_ptr<AttackObjectManager> attackObjectManger_;
	// エフェクトマネージャ
	std::unique_ptr<GameEffectManager> gameEffectManager_;

	// DirectionalLight
	DirectionalLight directionalLight_{};
};