#pragma once

// C++
#include <memory>

// MyHedder
#include "Math/Utility/MathUtility.h"

// 前方宣言
class GameObject3D;

/// <summary>
/// 胴体
/// </summary>
class MechBody {
public:
	MechBody();
	~MechBody() = default;

	void Update();

	std::weak_ptr<GameObject3D> GetGameObject()const;



private:
	// オブジェクト
	std::weak_ptr<GameObject3D> body_;

};