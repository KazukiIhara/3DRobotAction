#pragma once

#include "Feature/SceneStateController/CombatSceneController/States/ICombatSceneState.h"

#include "Structs/SpriteStruct.h"

// 実装メモ
/*
	戦闘中に落下死が確定する高さまで到達したときにこのフェーズに移行する
	カメラのプレイヤー追従処理を切り、真下を向けて落ちていく様子を見せて、一定時間たったらYOUDIED
*/

/// <summary>
/// 落下死ステート
/// </summary>
class CombatSceneStateDropped : public ICombatSceneState {
public:
	enum class Phase {
		Drop,
		YouDied
	};
public:
	~CombatSceneStateDropped()override = default;
	void Enter(CombatSceneControl::StateContextRef ref)override;
	void Update(CombatSceneControl::StateContextRef ref)override;
	void Draw(CombatSceneControl::StateContextRef ref)override;
	void Exit(CombatSceneControl::StateContextRef ref)override;

private:
	float timer_ = 0.0f;

	SpriteData youDiedSprite_{};
	SpriteMaterialData youDiedSpriteMat_{};

};