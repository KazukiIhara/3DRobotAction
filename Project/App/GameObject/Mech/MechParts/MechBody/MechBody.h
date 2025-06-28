#pragma once

// C++
#include <memory>

// MyHedder
#include "Math/Utility/MathUtility.h"

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

private:
	void DirectionToCamera(MechCore* mechCore);

private:
	// オブジェクト
	std::weak_ptr<GameObject3D> body_;

};