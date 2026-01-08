#pragma once

// C++
#include <memory>

#include "GameObject/Mech/MechCore/MechCore.h"
#include "GameObject/EnemyAI/EnemyAI.h"

// 前方宣言
class AttackObjectManager;

// TODO: 今さらだが、Shared_ptrを使っている理由が殆どないし、逆に危険な気がする
// 次のリファクタのタイミングでShared_ptrを使っている部分をunique_ptrにする
// TODO: ほぼEnemyクラスとやってること同じなので処理をまとめる

/// <summary>
/// AIで動くプレイヤー
/// </summary>
class AIPlayer {
public:
	AIPlayer(AttackObjectManager* bulletManager);
	~AIPlayer() = default;

	void Initialize(AttackObjectManager* bulletManager, std::weak_ptr<MechCore> targetMech);

	void Update();

	void Draw();

	// 機体を取得
	std::weak_ptr<MechCore> GetMechCore();

	bool GetIsAIActive() const;

	void SetIsAIActive(bool isActive);

private:
	// 機体クラス
	std::shared_ptr<MechCore> mech_ = nullptr;

	// AIクラス
	std::unique_ptr<EnemyAI> ai_;

	// AIのアクティブフラグ
	bool isAIActive_ = true;
};