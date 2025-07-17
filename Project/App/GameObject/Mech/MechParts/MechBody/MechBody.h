#pragma once

// C++
#include <memory>

// MyHedder
#include "Math/Utility/MathUtility.h"
#include "GameCommon/GameCommon.h"

// 前方宣言
class GameObject3D;
class MechCore;

/// <summary>
/// 胴体
/// </summary>
class MechBody {
public:
	MechBody();
	~MechBody() = default;

	void Update(MechCore* mechCore);

	std::weak_ptr<GameObject3D> GetGameObject()const;

	AABBCollider GetCollider()const;

private:
	void DirectionToLockOnView(MechCore* mechCore);
	void UpdateCollider();

	void DrawCollider();
private:
	// オブジェクト
	std::weak_ptr<GameObject3D> body_;

	// コライダー
	AABBCollider collider_;

	const Vector3 kMin_ = { -0.5f,-1.0f,-0.5f };
	const Vector3 kMax_ = { 0.5f,0.8f,0.5f };

};