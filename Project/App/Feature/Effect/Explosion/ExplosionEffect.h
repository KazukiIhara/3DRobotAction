#pragma once
#include <memory>

#include "Feature/Effect/System/BaseGameEffect/BaseGameEffect.h"
#include "Structs/Primitive3DStruct.h"
#include "Math/Types/AllMathTypes.h"

/// <summary>
/// 爆発エフェクト
/// ひとまず赤と黒っぽいパーティクルを出す
/// </summary>
class ExplosionEffect :public BaseGameEffect {
public:
	ExplosionEffect(const Vector3& emitPos);
	~ExplosionEffect() = default;

	void Update()override;
	void Draw()override;

private:

};