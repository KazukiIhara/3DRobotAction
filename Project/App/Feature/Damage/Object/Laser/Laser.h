#pragma once

// 基底クラス
#include "Feature/Damage/Object/Base/BaseDamageObject.h"
#include "Feature/Effect/LaserEffect/LaserEffect.h"

// エフェクトマネージャ
class GameEffectManager;

/// <summary>
/// レーザー
/// </summary>
class Laser:public BaseDamageObject {
public:
	struct RefContext {
		DamageCollisionSystem* damageCollisionSystem;
		GameEffectManager* effectManager;
	};
public:
	struct InitParam {
		Vector3 emitPos{};
		Vector3 dir{};
		float speed = 0.0f;
		float life = 0.0f;
	};
public:
	Laser(const Laser::InitParam& initParam, RefContext ref);
	~Laser()override = default;

	void Update()override;
	void Draw()override;
	void Finalize()override;

private:
	Vector3 laserEndPos_;
	Vector3 dir_;
	float speed_;
	float life_;

	GameEffectManager* effectManager_;
	LaserEffect* laserEffect_;
};