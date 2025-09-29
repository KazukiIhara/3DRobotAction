#pragma once

// C++
#include <vector>

// 弾
#include "GameObject/Bullet/Bullet.h"

// 前方宣言
class AttackCollisionManager;

/// <summary>
/// 攻撃オブジェクトマネージャ
/// </summary>
class AttackObjectManager {
public:
	AttackObjectManager(AttackCollisionManager* attackColliderManager);
	~AttackObjectManager() = default;

	void Update();
	void Draw();

	const std::vector<Bullet> GetBullets();

	void AddBullet(
		const FriendlyTag& tag,
		const Vector3& dir,
		float speed,
		const Vector3& wPos,
		int32_t damage);

private:
	// 弾のリスト
	std::vector<Bullet> bullets_;

private:
	AttackCollisionManager* atkColliderManager_ = nullptr;

};