#pragma once

#include "3D/Colliders3D/BaseCollider3D/BaseCollider3D.h"

/// <summary>
/// 球体コライダー
/// </summary>
class SphereCollider:public BaseCollider3D {
public:
	SphereCollider(const std::string& name,uint32_t id, Collider3DType type);
	~SphereCollider()override = default;

	void TransferShape()override;

	float& GetRadius();

private:
	float radius_ = 1.0f;
};