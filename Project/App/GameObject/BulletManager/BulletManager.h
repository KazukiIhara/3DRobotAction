#pragma once

// C++
#include <vector>

// 弾
#include "GameObject/Bullet/Bullet.h"

// 前方宣言
class AttackCollisionManager;

/// <summary>
/// 弾のマネージャ (弾だけでなく今後攻撃全般のマネージャになりそう)
/// </summary>
class BulletManager {
public:
	BulletManager(AttackCollisionManager* attackColliderManager);
	~BulletManager() = default;

	void Update();
	void Draw();

	void AddBullet(const FriendlyTag& tag, const Vector3& dir, float speed, const Vector3& wPos);

private:
	// 弾のリスト
	std::vector<Bullet> bullets_;

private:
	AttackCollisionManager* atkColliderManager_ = nullptr;

};