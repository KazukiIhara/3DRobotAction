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
	void Draw() const;

	Vector3 GetWorldPos()const;
	float GetRadius()const;
	const bool GetIsHit()const;

	void SetIsHit(bool isHit);

private:

	bool isHit_ = false;

	Vector3 worldPos_{};
	float radius_{};
};