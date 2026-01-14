#pragma once

// C++
#include <memory>

// MyHeader
#include "Math/Utility/MathUtility.h"
#include "GameObject/AttackObjects/BaseAttackObject/BaseAttackObject.h"
#include "Structs/Primitive3DStruct.h"

// 前方宣言
class Transform3D;

// 実装メモ
/*
	
*/

/// <summary>
/// レーザー攻撃
/// </summary>
class Laser:public BaseAttackObject {
public:
	Laser(
		const Vector3& pos,
		const Vector3& dir,
		float speed,
		AttackCollider* collider
	);
	~Laser() = default;

	void Update();
	void Draw();

private:
	// 発射地点のトランスフォーム
	Transform3D* baseTransform_;

};