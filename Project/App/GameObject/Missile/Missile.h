#pragma once

// C++
#include <memory>

// MyHedder
#include "Math/Utility/MathUtility.h"
#include "GameObject/AttackCollider/AttackCollider.h"

// 前方宣言
class GameObject3D;

// ミサイルの種類
enum MissileType {
	Dual,
};


/// <summary>
/// ミサイル
/// </summary>
class Missile {
public:
	Missile();
	~Missile() = default;

private:
	// ゲームオブジェクト
	std::weak_ptr<GameObject3D> missile_;

};