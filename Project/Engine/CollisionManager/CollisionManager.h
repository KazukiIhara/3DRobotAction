#pragma once

// C++
#include <map>
#include <functional>
#include <vector>
#include <set>

// MyHedder
#include "3D/Colliders3D/BaseCollider3D/BaseCollider3D.h"

class ColliderManager;

/// <summary>
/// コリジョンマネージャ
/// </summary>
class CollisionManager {
public:
	CollisionManager(ColliderManager* colliderManager);
	~CollisionManager() = default;

	// 更新
	void Update();

private:
	// チェックするコライダーを追加
	void Add(BaseCollider3D* baseCollider3D);
	// チェックするすべてのコライダーを追加
	void AddCheckColliders();
	// IDをもとにコライダーを取得する
	BaseCollider3D* FindColliderByID(const std::vector<BaseCollider3D*>& colliders, uint32_t id);
	// ペア同士の当たり判定をチェック
	bool CheckCollisionPair(BaseCollider3D* colliderA, BaseCollider3D* colliderB);
	// すべてのコライダー同士の当たり判定チェック
	std::set<std::pair<uint32_t, uint32_t>> CheckAllCollisions();
	// 衝突応答を呼ぶ
	void ResolveCollisions(const std::set<std::pair<uint32_t, uint32_t>>& currentCollisions);
	// コライダーリストをクリア
	void Clear();

	// ペアの順序を固定する関数
	std::pair<uint32_t, uint32_t> MakeOrderedPair(uint32_t idA, uint32_t idB);
	std::pair<Collider3DType, Collider3DType> MakeOrderedPair(Collider3DType typeA, Collider3DType typeB);

	// 
	// 当たり判定チェック処理
	// 

	// 同形状同士

	// 球体同士の当たり判定
	bool CheckSphereToSphereCollision(BaseCollider3D* colliderA, BaseCollider3D* colliderB);

	// AABB同士の当たり判定
	bool CheckAABBToAABBCollision(BaseCollider3D* colliderA, BaseCollider3D* colliderB);

	// 異種形状同士

	// 球体とAABBの当たり判定
	bool CheckSphereToAABBCollision(BaseCollider3D* colliderA, BaseCollider3D* colliderB);


private:
	// 衝突判定関数の型の定義
	using CollisionFunc = std::function<bool(BaseCollider3D*, BaseCollider3D*)>;
	// 衝突判定関数を管理するマップ
	std::map<std::pair<Collider3DType, Collider3DType>, CollisionFunc> collisionFuncMap_;

	// 衝突応答をチェックするリスト
	std::vector<BaseCollider3D*> activeColliders_;

	// 前フレームで衝突していたペア
	std::set<std::pair<uint32_t, uint32_t>> previousCollisions_;

private:
	// コライダーマネージャのポインタ
	ColliderManager* colliderManager_ = nullptr;
};