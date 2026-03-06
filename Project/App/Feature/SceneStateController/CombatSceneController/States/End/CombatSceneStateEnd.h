#pragma once

#include "Feature/SceneStateController/CombatSceneController/States/ICombatSceneState.h"

#include "Structs/SpriteStruct.h"

/// <summary>
/// 戦闘シーン終了ステート(多分勝利ステートに変更する)
/// </summary>
class CombatSceneStateEnd: public ICombatSceneState {
public:
	enum class Phase {
		Flash,
		Destroy,
		Explosion,
		Finish,
	};
public:
	~CombatSceneStateEnd()override = default;
	void Enter(CombatSceneControl::StateContextRef ref)override;
	void Update(CombatSceneControl::StateContextRef ref)override;
	void Draw(CombatSceneControl::StateContextRef ref)override;
	void Exit(CombatSceneControl::StateContextRef ref)override;

private:
	void UpdateFlash(CombatSceneControl::StateContextRef ref);
	void DrawFlash(CombatSceneControl::StateContextRef ref);

	void UpdateDestroy(CombatSceneControl::StateContextRef ref);
	void DrawDestroy(CombatSceneControl::StateContextRef ref);

	void UpdateExplosion(CombatSceneControl::StateContextRef ref);
	void DrawExplosion(CombatSceneControl::StateContextRef ref);

	void UpdateFinish(CombatSceneControl::StateContextRef ref);
	void DrawFinish(CombatSceneControl::StateContextRef ref);
private:
	Phase phase_ = Phase::Flash;
	float timer_ = 0.0f;

	SpriteData flashSprite_{};
	SpriteMaterialData flashSpriteMat_{};
};
