#pragma once

// C++
#include <vector>
#include <memory>

// 攻撃コライダー
#include "GameObject/AttackCollider/AttackCollider.h"

// 前方宣言
class MechCore;

/// <summary>
/// 攻撃コリジョンマネージャ
/// </summary>
class AttackCollisionManager {
public:
	AttackCollisionManager();
	~AttackCollisionManager() = default;

	void Update();
	void Draw();

	void CheckCollision();

	void AddMech(std::weak_ptr<MechCore> mechCore);
	std::weak_ptr<AttackCollider> AddAttackCollider(std::shared_ptr<AttackCollider> collider);


private:
	// AABB同士の当たり判定
	bool IsCollisionAABB(const magi::Vector3& minA, const magi::Vector3& maxA, const magi::Vector3& minB, const magi::Vector3& maxB);
	// 球体同士の当たり判定
	bool IsCollisionSphereToSphere(const magi::Vector3& posA, float radiusA, const magi::Vector3& posB, float radiusB);
	// AABBと球体の当たり判定
	bool IsCollisionAABBToSphere(const magi::Vector3& minA, const magi::Vector3& maxA, const magi::Vector3& posB, float radiusB);

private:
	// 機体のリスト
	std::vector<std::weak_ptr<MechCore>> mechs_;
	// 攻撃コライダーのリスト
	std::vector<std::shared_ptr<AttackCollider>> attackColliders_;

};