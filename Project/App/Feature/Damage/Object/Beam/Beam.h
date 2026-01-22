#pragma once

// 基底クラス
#include "Feature/Damage/Object/Base/BaseDamageObject.h"

// エフェクトマネージャ
class GameEffectManager;

// 実装メモ
/*
	地形に当たったら爆発とかやりたくない？俺はやりたい
*/

/// <summary>
/// ビーム
/// </summary>
class Beam :public BaseDamageObject {
public:
	struct RefContext {
		DamageCollisionSystem* damageCollisionSystem;
		GameEffectManager* effectManager;
	};
	struct InitParam {
		Vector3 emitPos{};
		Vector3 dir{};
		float speed = 0.0f;
		float life = 0.0f;
		float thickness = 0.0f;
		FriendlyTag tag;
	};
public:
	Beam(const Beam::InitParam& initParam, RefContext ref);
	~Beam()override = default;

	void Update()override;
	void Draw()override;
	void Finalize()override;
private:
	Vector3 beamEndPos_;
	Vector3 dir_;
	float speed_;
	float life_;
	float thickness_;

	GameEffectManager* effectManager_;
};