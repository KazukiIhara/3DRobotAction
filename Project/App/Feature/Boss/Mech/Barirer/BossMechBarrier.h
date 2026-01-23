#pragma once

// C++
#include <cstdint>

#include "Feature/Mech/Parts/IMechParts.h"

#include "3D/Transform3D/Transform3D.h"

#include "Structs/Primitive3DStruct.h"
#include "Structs/ModelStruct.h"

// ボス機体
class BossMech;

// 実装メモ
/*

*/

/// <summary>
/// ボスのバリア
/// </summary>
class BossMechBarrier: public IMechParts {
	struct State {
		// 有効フラグ
		bool isActive_ = true;
		// 耐久力
		float hp = 0.0f;
	};
	struct Collider {
		Vector3 wPos{};
		float radius = 0.0f;
	};
public:
	BossMechBarrier(BossMech* mech);
	~BossMechBarrier() = default;

	void Update()override;

	void Draw()override;

	// ダメージを与える
	void Damage(float damage);
	// バリア復活
	void RepairBarrier();

	const State& GetState();

	const Collider& GetCollider();

private:
	Transform3D* transform_;
	SphereData3D data_;
	MaterialData3D mat_;

	State state_{};
	Collider collider_{};
	BossMech* mech_ = nullptr;
};