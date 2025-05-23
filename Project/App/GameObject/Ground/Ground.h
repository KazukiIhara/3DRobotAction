#pragma once

// C++
#include <memory>

// MyHedder
#include "Transform3D/Transform3D.h"
#include "Framework/MAGI.h"

/// <summary>
/// 地面
/// </summary>
class Ground {
public:
	Ground();
	~Ground() = default;

	void Update();

	void Draw();

private:
	// 板ポリデータ
	PlaneData3D planeData_;

	// トランスフォーム
	std::unique_ptr<Transform3D> transform_;

};
