#pragma once

// C++
#include <memory>

// MyHedder
#include "Math/Utility/MathUtility.h"

// 前方宣言
class GameObject3D;
class Transform3D;
class MechCore;

/// <summary>
/// 左腕
/// </summary>
class MechArmLeft {
public:
	MechArmLeft();
	~MechArmLeft() = default;

	void Update(MechCore* mechCore);

	std::weak_ptr<GameObject3D> GetGameObject()const;

	Transform3D* GetHandTransform();

	const Vector3& GetForward()const;

	void DebugDraw();

private:
	// オブジェクト
	std::weak_ptr<GameObject3D> leftArm_;
	// 前方ベクトル
	Vector3 forward_ = { 0.0f,0.0f,1.0f };

	// 肘のトランスフォーム
	Transform3D* elbowTransform_;

	// 手のトランスフォーム
	Transform3D* handTransform_;
};