#pragma once

#include <string>
#include "Math/Utility/MathUtility.h"

/// <summary>
/// ワールド上に存在するものの基底クラス
/// </summary>
class WorldEntity {
public:
	WorldEntity();
	virtual ~WorldEntity() = default;

public:
	// 名前
	std::string name;
	// ワールド座標
	Vector3 worldPosition{};
	// 生存フラグ
	bool isAlive = true;
};