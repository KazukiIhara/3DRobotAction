#pragma once

#include <vector>

#include "GameObject/Bullet/Bullet.h"

/// <summary>
/// 弾のマネージャ
/// </summary>
class BulletManager {
public:
	BulletManager();
	~BulletManager() = default;

	void Update();
	void Draw();

	void AddBullet(const FriendlyTag& tag, const Vector3& dir, float speed, const Vector3& wPos);

private:
	// 弾のリスト
	std::vector<Bullet> bullets_;

};