#pragma once

#include <array>

// 基底クラス
#include "Feature/Damage/Object/ProjectileAttack/Base/BaseProjectileAttack.h"

#include "Structs/Primitive3DStruct.h"

// エフェクトマネージャ
class GameEffectManager;

/// <summary>
/// ビームキャノン
/// </summary>
class BeamCannon:public BaseProjectileAttack {
public:
	struct RefContext {
		DamageCollisionSystem* damageCollisionSystem;
		GameEffectManager* effectManager;
	};
	struct InitParam {
		BaseProjectileAttack::InitParam initParam_;
	};
public:
	BeamCannon(const BeamCannon::InitParam& initParam, RefContext ref);
	~BeamCannon()override = default;

	void Update()override;
	void Draw()override;
	void Finalize()override;


private:
	GameEffectManager* effectManager_;

	Magi::Transform3D* transParent_;
	std::array<Magi::Transform3D*, 4> trans_;
	MaterialData3D mat_;
	MaterialData3D matBlue_;
};