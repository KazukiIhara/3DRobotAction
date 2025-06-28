#pragma once

// C++
#include <memory>

// MyHedder
#include "Math/Utility/MathUtility.h"

// 前方宣言
class GameObject3D;

class MechLeg {
public:
	MechLeg();
	~MechLeg() = default;

	void Update();

	std::weak_ptr<GameObject3D> GetGameObject()const;

private:
	// オブジェクト
	std::weak_ptr<GameObject3D> leg_;

};