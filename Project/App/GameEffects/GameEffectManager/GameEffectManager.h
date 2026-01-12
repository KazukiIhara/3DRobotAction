#pragma once

// C++
#include <memory>
#include <vector>

#include "GameEffects/BaseGameEffect/BaseGameEffect.h"

// 実装メモ
/*
	エフェクトの追加更新描画、生存フラグがFalseのものの削除を行う
	シーンに配置し、エフェクトを発生させる各オブジェクトがこれの参照ポインタを持つ設計
*/

/// <summary>
/// ゲームエフェクトマネージャ
/// </summary>
class GameEffectManager {
public:
	GameEffectManager();
	~GameEffectManager() = default;

	void Add(std::unique_ptr<BaseGameEffect> effect);
	void Update();
	void Draw();
	void DeleteGarbage();

private:
	// エフェクトコンテナ
	std::vector<std::unique_ptr<BaseGameEffect>> effects_;
};