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

// ミサイルのフェーズ
enum MissilePhase {
	Boost,
	Guided,
};

/// <summary>
/// ミサイル
/// </summary>
class Missile {
public:
	Missile(
		const Vector3& wPos,
		float speed,
		const Vector3& dir,
		std::weak_ptr<AttackCollider> attackCollider
	);
	~Missile() = default;

	void Update();

	void Finalize();

	bool GetIsAlive()const;

	AttackCollider* GetAttackCollider();

	Vector3 GetWorldPos();

private:
	// ゲームオブジェクト
	std::weak_ptr<GameObject3D> missile_;

	// ミサイルの種類
	MissileType type_;

	// 進行方向
	Vector3 dir_ = { 0.0f,0.0f,0.0f };
	// 速度
	float speed_ = 0.0f;

	// 生存フラグ
	bool isAlive_ = false;

	// ブースト時の加速量
	float boostAcc_ = 3.0f;

	// 最大生存時間
	float lifeTime_ = 10.0f;

	// 攻撃コライダー
	std::weak_ptr<AttackCollider> collider_;
};