#pragma once

// C++
#include <memory>

// MyHedder
#include "Math/Utility/MathUtility.h"

// 前方宣言
class GameObject3D;
class MechCore;

/// <summary>
/// 右腕
/// </summary>
class MechArmRight {
public:
	MechArmRight();
	~MechArmRight() = default;

	void Update(MechCore* mechCore);

	std::weak_ptr<GameObject3D> GetGameObject()const;

private:
	// オブジェクト
	std::weak_ptr<GameObject3D> rightArm_;

};