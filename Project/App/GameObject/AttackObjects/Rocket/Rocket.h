#pragma once

// C++
#include <memory>

// MyHedder
#include "Math/Utility/MathUtility.h"
#include "GameObject/AttackCollider/AttackCollider.h"
#include "Structs/Primitive3DStruct.h"

// 前方宣言
class Transform3D;

/// <summary>
/// ロケット弾
/// </summary>
class Rocket {
public:
	Rocket(const magi::Vector3& dir,
		float speed,
		const magi::Vector3& wPos,
		std::weak_ptr<AttackCollider> attackCollider);

	~Rocket() = default;

	void Update();
	void Draw();

	void Finalize();

	bool GetIsAlive()const;

	AttackCollider* GetAttackCollider();

	magi::Vector3 GetWorldPos();

private:
	// トランスフォーム
	Transform3D* transform_;
	// 球体データ
	SphereData3D sphereData_;
	// マテリアル
	MaterialData3D material_;
	// 弾の向き
	magi::Vector3 dir_ = { 0.0f,0.0f,0.0f };
	// 弾の速度
	float speed_ = 0.0f;

	// 最大生存時間
	float lifeTime_ = 5.0f;
	// 生存フラグ
	bool isAlive_ = false;

	// 攻撃コライダー
	std::weak_ptr<AttackCollider> collider_;
};