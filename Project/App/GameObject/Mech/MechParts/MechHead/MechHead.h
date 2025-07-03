#pragma once

// C++
#include <memory>

// MyHedder
#include "Math/Utility/MathUtility.h"

// 前方宣言
class GameObject3D;
class MechCore;

/// <summary>
/// 頭
/// </summary>
class MechHead {
public:
	MechHead();
	~MechHead() = default;

	void Update(MechCore* mechCore);

	std::weak_ptr<GameObject3D> GetGameObject()const;

private:
	// オブジェクト
	std::weak_ptr<GameObject3D> head_;

};