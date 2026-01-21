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

private:
	PilotMech* mech_;
	Sphere collider_;
};