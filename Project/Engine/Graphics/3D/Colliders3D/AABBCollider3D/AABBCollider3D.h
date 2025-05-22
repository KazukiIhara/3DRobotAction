#pragma once

#include "3D/Colliders3D/BaseCollider3D/BaseCollider3D.h"

/// <summary>
/// AABBコライダー
/// </summary>
class AABBCollider :public BaseCollider3D {
public:
	AABBCollider(const std::string& name, uint32_t id, Collider3DType type);
	~AABBCollider()override = default;

	void TransferShape()override;

	// オフセット最小値を取得
	Vector3& GetOffsetMin();
	// オフセット最大値を取得
	Vector3& GetOffsetMax();

	// 最小値を取得
	Vector3 GetMin()const;
	// 最大値を取得
	Vector3 GetMax()const;

private:
	// AABBの最小オフセット
	Vector3 offsetMin_{ -1.0f,-1.0f,-1.0f };
	// AABBの最大オフセット
	Vector3 offsetMax_{ 1.0f,1.0f,1.0f };
};