#pragma once

// C++
#include <cstdint>

#include "Feature/Mech/Parts/IMechParts.h"

#include "3D/Transform3D/Transform3D.h"

#include "Structs/Primitive3DStruct.h"
#include "Structs/ModelStruct.h"

// ボス機体
class BossMech;

/// <summary>
/// ボスのバリア
/// </summary>
class BossMechBarrier: public IMechParts {
	struct Status {
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

	const Status& GetState();

	const Collider& GetCollider();

private:
	Magi::Transform3D* transform_;
	SphereData3D data_;
	MaterialData3D mat_;

	Status status_{};
	Collider collider_{};
	BossMech* mech_ = nullptr;
};
