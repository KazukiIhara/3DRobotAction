#pragma once

// C++
#include <vector>

// 弾
#include "GameObject/Bullet/Bullet.h"
// ミサイル
#include "GameObject/Missile/Missile.h"

// 前方宣言
class AttackCollisionManager;
class MechCore;

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

	void AddMissile(
		const FriendlyTag& tag,
		const MissileType& missileType,
		const Vector3& wPos,
		float speed,
		float acc,
		float maxSpeed,
		const Vector3& dir,
		int32_t damage,
		std::weak_ptr<MechCore> target
	);

private:
	// 弾のリスト
	std::vector<Bullet> bullets_;
	// ミサイルのリスト
	std::vector<Missile> missiles_;

private:
	AttackCollisionManager* atkColliderManager_ = nullptr;

};