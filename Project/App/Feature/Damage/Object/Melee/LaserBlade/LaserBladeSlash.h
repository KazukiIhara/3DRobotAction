#pragma once

// C++
#include <array>


// 基底クラス
#include "Feature/Damage/Object/Melee/Base/BaseMelee.h"

#include "Structs/Primitive3DStruct.h"

// エフェクトマネージャ
class GameEffectManager;

/// <summary>
/// レーザーブレード斬撃
/// </summary>
class LaserBladeSlash:public BaseMelee {
public:
	struct RefContext {
		DamageCollisionSystem* damageCollisionSystem;
		GameEffectManager* effectManager;
	};
	struct InitParam {
		BaseMelee::InitParam initParam_;
	};
public:
	LaserBladeSlash(const LaserBladeSlash::InitParam& initParam, RefContext ref);
	~LaserBladeSlash() = default;

	void Update()override;
	void Draw()override;
	void Finalize()override;

private:
	GameEffectManager* effectManager_;

};