#pragma once

#include "Math/Types/AllMathTypes.h"

class MechCore;

/// <summary>
///　ジャスト回避用のコライダー
/// </summary>
class MechJustDodgeCollider {
public:
	MechJustDodgeCollider();
	~MechJustDodgeCollider() = default;

	void Update(MechCore* core);

	Vector3 GetWorldPos()const;
	float GetRadius()const;

	void Draw() const;

private:
	Vector3 worldPos_{};
	float radius_{};
};