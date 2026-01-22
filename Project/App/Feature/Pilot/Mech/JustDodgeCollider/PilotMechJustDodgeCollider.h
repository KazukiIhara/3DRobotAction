#pragma once

// C++
#include "Math/Types/Vector3.h"

// 機体クラス
class PilotMech;

/// <summary>
/// ジャスト回避コライダー
/// </summary>
class PilotMechJustDodgeCollider {
public:
	struct Sphere {
		Vector3 center;
		float radius;
	};
public:
	PilotMechJustDodgeCollider(PilotMech* mech);
	~PilotMechJustDodgeCollider() = default;

	void Update();
	void Draw();

	Sphere GetCollider();

	void SetIsHit(bool isHit);
	bool GetIsHit() const;

private:
	bool isHit_ = false;

	Sphere collider_;
	PilotMech* mech_;
};